#pragma once
#include "fwddecl.h"

#include <memory>
#include "Tracer.h"
#include "Sampler.h"

namespace Cruisky {
	namespace RayTracer {

		struct RendererConfig {
			RendererConfig(){}
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