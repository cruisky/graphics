#pragma once
#include "fwddecl.h"

#include <memory>
#include "Tracer.h"
#include "Sampler.h"
#include "Config.h"

namespace Cruisky {
	namespace RayTracer {

		struct RendererConfig {
			RendererConfig(TracerType tracer_type = TracerType::DirectLighting, int spp = 1, SamplerType sampler_type = SamplerType::Random) : 
				tracer_t(tracer_type), samples_per_pixel(spp), sampler_t(sampler_type){}
			int samples_per_pixel;
			TracerType tracer_t;
			SamplerType sampler_t;
		};


		class Renderer {
		public:
			Renderer(const RendererConfig& config);

			void Render(const Scene *scene, const Camera *camera, Film *film, shared_ptr<IProgressMonitor> progress_monitor = nullptr);
			inline const RendererConfig& Config(){ return config; }
		private:
			RendererConfig config;
			unique_ptr<Tracer> tracer_;
			unique_ptr<Sampler> sampler_;
		};
	}
}