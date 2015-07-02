#include "stdafx.h"
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "Film.h"
#include "Sample.h"
#include "Color.h"
#include "Ray.h"
#include "Config.h"
#include "Tools.h"

#include "Tracers/DirectLighting.h"
#include "Tracers/PathTracer.h"
#include "Samplers/RandomSampler.h"

namespace TX {
	namespace RayTracer {
		Renderer::Renderer(const RendererConfig& config, shared_ptr<Scene> scene, shared_ptr<Film> film)
			: scene(scene), film(film) {
			switch (config.tracer_t){
			case TracerType::DirectLighting:
				tracer_.reset(new DirectLighting);
				break;
			case TracerType::PathTracing:
			default:
				tracer_.reset(new PathTracer);
			}

			switch (config.sampler_t){
			case SamplerType::Random:
			default:
				sampler_.reset(new RandomSampler);
			}

			Config(config);
			thread_sync_.Init(config_.width, config_.height);
		}

		Renderer& Renderer::Config(const RendererConfig& config){
			Resize(config.width, config.height);
			config_ = config;
			return *this;
		}

		Renderer& Renderer::Resize(int width, int height) {
			if (config_.width != width || config_.height != height){
				config_.width = width;
				config_.height = height;
				scene->camera->Resize(width, height);
				film->Resize(width, height);
			}
			return *this;
		}

		void Renderer::Render(int workerId) {
			CameraSample cam_sample(10);
			Ray ray;
			Color c;
			RNG rng;
			assert(film->Width() == scene->camera->Width());
			assert(film->Height() == scene->camera->Height());
			tracer_->SetScene(scene.get());
			float spp_rec = 1.f / config_.samples_per_pixel;
			// stratefied sampling
			for (int p = 0; p < config_.samples_per_pixel; p++){
				for (int q = 0; q < config_.samples_per_pixel; q++){
					for (int y = 0; y < film->Height(); y++){
						for (int x = 0; x < film->Width(); x++){
							sampler_->GetSamples(&cam_sample);
							cam_sample.pix_x = x;
							cam_sample.pix_y = y;
							cam_sample.x = x + (p + cam_sample.x) * spp_rec;
							cam_sample.y = y + (q + cam_sample.y) * spp_rec;
							scene->camera->GenerateRay(&ray, cam_sample);
							tracer_->Trace(ray, cam_sample, rng, &c);
							film->Commit(cam_sample, c);
						}
					}
					film->ScalePixels();
				}
			}
			//film->ScalePixels();
		}
	}
}
