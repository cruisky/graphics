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
		
		class CameraSample {
		public:
			CameraSample(int bufsize);
			// Returns a pointer to an array of at least <count> samples
			Sample *RequestSamples(int count) const;
			~CameraSample();
		public:
			int pix_x, pix_y;
			float x, y;		// [0, 1] after sampling
			const int bufsize;
			Sample * const buffer;
		private:
			mutable int offset_ = 0;
		};


		inline float BalanceHeuristic(int nf, float f_pdf, int ng, float g_pdf) {
			return (nf * f_pdf) / (nf * f_pdf + ng * g_pdf);
		}

		inline float PowerHeuristic(int nf, float f_pdf, int ng, float g_pdf) {
			float f = nf * f_pdf, g = ng * g_pdf, f2 = f * f;
			return f2 / (f2 + g * g);
		}
	}
}