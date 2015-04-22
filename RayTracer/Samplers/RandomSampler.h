#pragma once

#include "Core/Sampler.h"

namespace Cruisky{
	namespace RayTracer{
		class RandomSampler : public Sampler {
		public:
			RandomSampler(){}
			~RandomSampler(){}

			void GetSamples(CameraSample *sample);
		private:
			RNG rng;
		};
	}
}