#pragma once
#include "fwddecl.h"
#include <iostream>
#include "Math/Random.h"

namespace TX
{
	namespace RayTracer
	{
		class Sampler {
		public:
			virtual ~Sampler(){}
			
			// Fills all fields with canonical random value
			virtual void GetSamples(CameraSample *sample) = 0;
		};
	}
}