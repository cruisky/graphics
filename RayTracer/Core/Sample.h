#pragma once 

#include "fwddecl.h"
#include <iostream>

namespace Cruisky{
	namespace RayTracer{
		struct Sample {
			float u, v, w; 
			Sample(){}
			Sample(RNG& rng);
			// all values are within range [0, 1]
			Sample(float u, float v, float w);
		};
		std::ostream& operator<<(std::ostream& os, const Sample& sample);
		
		struct CameraSample {
			float x, y;		// [0, 1]
			const int bufsize;
			Sample * const buffer;
			CameraSample(int bufsize);
			~CameraSample();
		};
	}
}