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
			// Sample buffer
			sample_buf_ = std::make_unique<CameraSample>(10);	// should be enough to trace a ray
			// Config renderer
			Config(config);
			// Init tiled rendering synchronizer
			thread_sync_.Init(config_.width, config_.height);
		}

		Renderer& Renderer::Config(const RendererConfig& config){
			Resize(config.width, config.height);
			switch (config.tracer_t){
			case TracerType::DirectLighting:
				tracer_ = std::make_unique<DirectLighting>(scene.get());
				break;
			case TracerType::PathTracing:
			default:
				tracer_ = std::make_unique<PathTracer>(scene.get());
			}
			// generate sample offset for current tracer
			tracer_->BakeSamples(scene.get(), sample_buf_.get());

			switch (config.sampler_t){
			case SamplerType::Random:
			default:
				sampler_.reset(new RandomSampler);
			}
			config_ = config;
			return *this;
		}

		Renderer& Renderer::Resize(int width, int height) {
			if (config_.width != width || config_.height != height){
				config_.width = width;
				config_.height = height;
				scene->camera->Resize(width, height);
				film->Resize(width, height);
				thread_sync_.Init(width, height);
			}
			return *this;
		}

		void Renderer::Abort(){
			thread_sync_.Abort();
			ThreadScheduler::Instance()->JoinAll();
		}

		void Renderer::NewTask(){
			film->Reset();
			thread_sync_.Resume();
			for (auto i = 0; i < ThreadScheduler::Instance()->ThreadCount(); i++){
				tasks_.push_back(std::make_shared<RenderTask>(this));
				ThreadScheduler::Instance()->AddTask(Task((Task::Func)&RenderTask::Run, tasks_[i].get()));
			}
		}

		void Renderer::Render(int workerId, RNG& random) {
			// duplicate sample buffer in each thread
			CameraSample sample_buf_dup(*sample_buf_);
			for (int i = 0; thread_sync_.Running() && i < config_.samples_per_pixel; i++){
				// sync threads before and after each sample frame
				thread_sync_.PreRenderSync(workerId);
				RenderTiles(sample_buf_dup, random);
				thread_sync_.PostRenderSync(workerId);

				if (workerId == 0){
					film->ScalePixels();
					thread_sync_.ResetTiles();
				}
			}
		}

		void Renderer::RenderTiles(CameraSample& sample_buf, RNG& random){
			RenderTile* tile;
			Ray ray;
			Color c;
			while (thread_sync_.NextTile(tile)){
				for (int y = tile->ymin; y < tile->ymax; y++){
					for (int x = tile->xmin; x < tile->xmax; x++){
						if (!thread_sync_.Running()) return;
						sampler_->GetSamples(&sample_buf);
						sample_buf.pix_x = x;
						sample_buf.pix_y = y;
						sample_buf.x = x + sample_buf.x;
						sample_buf.y = y + sample_buf.y;
						scene->camera->GenerateRay(&ray, sample_buf);
						tracer_->Trace(ray, sample_buf, random, &c);
						film->Commit(sample_buf, c);
					}
				}
			}
		}
	}
}
