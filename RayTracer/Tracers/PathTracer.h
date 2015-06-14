#pragma once

#include "fwddecl.h"
#include "Core/Tracer.h"


namespace TX{
	namespace RayTracer{
		class PathTracer : public Tracer {
		public:
			PathTracer(int maxdepth = 5) : Tracer(maxdepth){}
			~PathTracer(){}

		protected:
			Color Li(const Ray& ray, int depth);
			void ProcessSamples();
		private:
			static const int SAMPLE_DEPTH;
			std::vector<Sample *> light_samples_;
			std::vector<Sample *> bsdf_samples_;
			std::vector<Sample *> scatter_samples_;
		};
	}
}