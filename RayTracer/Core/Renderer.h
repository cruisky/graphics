#pragma once
#include "fwddecl.h"

#include <memory>
#include "Tracer.h"
#include "Synchronizer.h"
#include "Sampler.h"
#include "Sample.h"
#include "Config.h"

namespace TX {
	namespace RayTracer {

		struct RendererConfig {
			RendererConfig(){}
			int samples_per_pixel;
			int width = 0, height = 0;
			TracerType tracer_t;
			SamplerType sampler_t;
		};

		class Renderer {
		public:
			Renderer(const RendererConfig& config, shared_ptr<Scene> scene, shared_ptr<Film> film);
			void Abort();
			void NewTask();
			void Render(int workerId, RNG& random);
			void RenderTiles(CameraSample& sample_buf, RNG& random);

			inline const RendererConfig& Config(){ return config_; }
			Renderer& Config(const RendererConfig& config);
			Renderer& Resize(int width, int height);
		public:
			shared_ptr<Scene> scene;
			shared_ptr<Film> film;
		private:
			RendererConfig config_;
			unique_ptr<Tracer> tracer_;
			unique_ptr<Sampler> sampler_;
			unique_ptr<CameraSample> sample_buf_;
			Synchronizer thread_sync_;
			vector<shared_ptr<RenderTask>> tasks_;
		};
	}
}
