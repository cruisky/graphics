#pragma once
#include "fwddecl.h"

#include <memory>
#include "Tracer.h"
#include "Sampler.h"

namespace Cruisky {
	namespace RayTracer {
		class Renderer{
		public:
			Renderer();

			void Render(const Scene *scene, const Camera *camera, Film *film);
		
		private:
			unique_ptr<Tracer> tracer_;
			unique_ptr<Sampler> sampler_;
		};
	}
}