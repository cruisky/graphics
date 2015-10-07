#pragma once
#include "Util.h"
#include <iostream>
#include "Math/Random.h"

namespace TX
{
	class Sampler {
	public:
		virtual ~Sampler(){}
			
		// Fills all fields with canonical random value
		virtual void GetSamples(CameraSample *sample) = 0;
	};
}