#pragma once
#include "fwddecl.h"

#include <memory>
#include "Tracer.h"
#include "Sampler.h"

namespace Cruisky {
	namespace RayTracer {
		class Renderer{
		public:
			Renderer(const Scene *scene, const Camera *camera, Film *film);
		private:
			const Scene* scene_;
			const Camera* camera_;
			Film* film_;
			std::unique_ptr<Tracer> tracer_;
			std::unique_ptr<Sampler> sampler_;
			//TODO
		};
	}
}