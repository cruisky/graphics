#pragma once
#include "fwddecl.h"

#include <memory>
#include "Tracer.h"
#include "Sampler.h"

namespace Cruisky {
	namespace RayTracer {
		class Renderer{
		public:
			Renderer(Film *film);

			void Render(const Scene *scene, const Camera *camera);
		private:
			Film* film_;
			std::unique_ptr<Tracer> tracer_;
			std::unique_ptr<Sampler> sampler_;
		};
	}
}