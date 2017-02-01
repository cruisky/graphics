#pragma once

#include "Core/Sampler.h"

namespace TX{
	class RandomSampler : public Sampler {
	public:
		RandomSampler(){}
		~RandomSampler(){}

		void GetSamples(CameraSample *sample);
	private:
		RNG rng;
	};
}
