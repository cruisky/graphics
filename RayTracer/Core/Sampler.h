#pragma once
#include "fwddecl.h"
#include <iostream>
#include "RNG.h"

namespace Cruisky
{
	namespace RayTracer
	{
		class Sampler {
		public:
			virtual ~Sampler(){}

			// Generates a camera sample and return true.
			// Return false when all samples are generated.
			virtual void GetSamples(CameraSample *sample) = 0;
		};


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