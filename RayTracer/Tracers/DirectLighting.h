#pragma once

#include "fwddecl.h"
#include "Core/Tracer.h"
#include <vector>

namespace TX{
	namespace RayTracer{
		class DirectLighting : public Tracer {
		public:
			DirectLighting(int maxdepth = 5);
			~DirectLighting(){}

			void BakeSamples(const Scene *scene, const CameraSample *samplebuf);
		protected:
			Color Li(const Scene *scene, const Ray& ray, int depth, const CameraSample& samplebuf);

		private:
			std::vector<SampleOffset> light_samples_;
			std::vector<SampleOffset> bsdf_samples_;
		};
	}
}
