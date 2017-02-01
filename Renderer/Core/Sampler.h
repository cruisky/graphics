#pragma once
#include "TXBase/fwddecl.h"
#include <iostream>
#include "TXBase/Math/Random.h"

namespace TX
{
	class Sampler {
	public:
		virtual ~Sampler(){}

		/// <summary>
		/// Fills all fields with canonical random value.
		/// </summary>
		virtual void GetSamples(CameraSample *sample) = 0;
	};
}
