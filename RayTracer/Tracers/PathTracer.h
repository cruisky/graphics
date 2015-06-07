#pragma once

#include "fwddecl.h"
#include "Core/Tracer.h"


namespace Cruisky{
	namespace RayTracer{
		class PathTracer : public Tracer {
		public:
			PathTracer(int maxdepth = 5) : Tracer(maxdepth){}
			~PathTracer(){}

		protected:
			Color Li(const Ray& ray, int depth);
			void ProcessSamples();
		private:
			std::vector<Sample *> light_samples_;
			std::vector<Sample *> bsdf_samples_;
			std::vector<Sample *> scatter_samples_;
		};
	}
}