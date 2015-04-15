#include "stdafx.h"

#include "Sampler.h"

namespace Cruisky{
	namespace RayTracer {
		std::ostream& operator<<(std::ostream& os, const Sample& sample){
			os << "(" << sample.u << "," << sample.v << ")";
			return os;
		}

		SimpleSampler::SimpleSampler(int xstart, int xend, int ystart, int yend) :
			Sampler(xstart, xend, ystart, yend, 1), x(xstart), y(ystart){}
	
		bool SimpleSampler::GetSample(Sample *sample, RNG *rng){
			if (x_start == x_end || y_start == y_end)
				return false;
			if (x == x_end) {
				x = x_start;
				y++;
			}
			if (y == y_end)
				return false;
			
			// simply take a sample at the center of the pixel
			sample->u = x++ + 0.5f;
			sample->v = y + 0.5f;
			return true;
		}
	}
}