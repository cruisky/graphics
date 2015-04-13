#pragma once
#include "fwddecl.h"

namespace Cruisky
{
	namespace RayTracer
	{
		struct Sample {
		public:
			Sample(){}
			Sample(float u, float v) : u(u), v(v) {}

		public:
			float u, v;
		};

		class Sampler {
		public:
			Sampler(){}
			// Generates a camera sample and return true.
			// Return false when all samples are generated.
			virtual bool GetSample(Sample *sample);
		};
	}
}