#pragma once
#include "fwddecl.h"

#include <memory>
#include "Tracer.h"
#include "Sampler.h"
#include "Config.h"

namespace Cruisky {
	namespace RayTracer {

		struct RendererConfig {
			RendererConfig(TracerType tracer_type = TracerType::DirectLighting, SamplerType sampler_type = SamplerType::Random) : 
				tracer_t(tracer_type), sampler_t(sampler_type){}
			TracerType tracer_t;
			SamplerType sampler_t;
		};


		class Renderer{
		public:
			
			Renderer(const RendererConfig& config);

			void Render(const Scene *scene, const Camera *camera, Film *film);
		
		private:
			RendererConfig config;
			unique_ptr<Tracer> tracer_;
			unique_ptr<Sampler> sampler_;
		};
	}
}