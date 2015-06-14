#pragma once
#include "fwddecl.h"
#include <iostream>
#include "RNG.h"

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