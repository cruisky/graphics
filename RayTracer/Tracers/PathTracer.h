#pragma once

#include "fwddecl.h"
#include "Core/Tracer.h"

namespace TX{
	namespace RayTracer{
		class PathTracer : public Tracer {
		public:
			PathTracer(int maxdepth = 6);
			~PathTracer(){}
			void BakeSamples(const Scene *scene, const CameraSample *samplebuf);
		protected:
			Color Li(const Scene *scene, const Ray& ray, int depth, const CameraSample& samplebuf);
		private:
			static const int SAMPLE_DEPTH;
			std::vector<SampleOffset> light_samples_;
			std::vector<SampleOffset> bsdf_samples_;
			std::vector<SampleOffset> scatter_samples_;
		};
	}
}
