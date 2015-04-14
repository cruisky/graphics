#pragma once
#include "fwddecl.h"

#include <memory>
#include "Scene.h"

namespace Cruisky {
	namespace RayTracer {
		class Renderer{
		public:
			//TODO
		private:
			std::shared_ptr<Camera> camera_;
			std::shared_ptr<Scene> scene_;
			std::unique_ptr<Tracer> tracer_;
			std::unique_ptr<Film> film_;
			//TODO
		};
	}
}