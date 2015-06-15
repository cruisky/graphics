#pragma once 

#include "fwddecl.h"
#include "Core/Tracer.h"
#include <vector>

namespace TX{
	namespace RayTracer{
		class DirectLighting : public Tracer {
		public:
			DirectLighting(int maxdepth = 5) : Tracer(maxdepth){}
			~DirectLighting(){}

		protected:
			Color Li(const Ray& ray, int depth);
			void ProcessSamples();

		private:
			std::vector<Sample *> light_samples_;
			std::vector<Sample *> bsdf_samples_;
		};
	}
}