#include "stdafx.h"
#include "Renderer.h"
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

namespace Cruisky {
	namespace RayTracer {
		Renderer::Renderer(const RendererConfig& config) : config(config){
			switch (config.tracer_t){
			case TracerType::DirectLighting:
				tracer_.reset(new DirectLighting);
				break;
			case TracerType::PathTracing:
				tracer_.reset(new PathTracer);
				break;
			}

			switch (config.sampler_t){
			case SamplerType::Random:
				sampler_.reset(new RandomSampler);
				break;
			}
		}

		void Renderer::Render(const Scene *scene, const Camera *camera, Film *film, shared_ptr<IProgressMonitor> monitor) {
			CameraSample cam_sample(10);
			Ray ray; 
			Color c;
			RNG rng;
			assert(film->Width() == camera->Width());
			assert(film->Height() == camera->Height());
			tracer_->SetScene(scene);
			if (monitor) monitor->Reset(film->Height());
			float spp_rec = 1.f / config.samples_per_pixel;
			for (int y = 0; y < film->Height(); y++){
				for (int x = 0; x < film->Width(); x++){
					// stratefied sampling
					for (int p = 0; p < config.samples_per_pixel; p++){
						for (int q = 0; q < config.samples_per_pixel; q++){
							sampler_->GetSamples(&cam_sample);
							cam_sample.pix_x = x;
							cam_sample.pix_y = y;
							cam_sample.x = x + (p + cam_sample.x) * spp_rec;
							cam_sample.y = y + (q + cam_sample.y) * spp_rec;
							camera->GenerateRay(&ray, cam_sample);
							tracer_->Trace(ray, cam_sample, rng, &c);
							film->Commit(cam_sample, c);
						}
					}
				}
				if (monitor) monitor->Update(y);
			}
			if (monitor) monitor->Finish();
			film->ScalePixels();
		}
	}
}