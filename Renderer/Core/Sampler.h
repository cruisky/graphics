#pragma once
#include <iostream>
#include "txbase/fwddecl.h"
#include "txbase/math/random.h"

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
