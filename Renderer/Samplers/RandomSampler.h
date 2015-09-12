#pragma once

#include "Rendering/Sampler.h"

namespace TX{
	namespace Rendering{
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