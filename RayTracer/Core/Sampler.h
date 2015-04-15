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
			// Includes start, exclude end
			Sampler(int xstart, int xend, int ystart, int yend, int spp) :
				x_start(xstart), x_end(xend), y_start(ystart), y_end(yend), sample_per_pix(spp){}
			virtual ~Sampler(){}

			// Generates a camera sample and return true.
			// Return false when all samples are generated.
			virtual bool GetSample(Sample *sample, RNG *rng) = 0;
		public:
			const int x_start, x_end, y_start, y_end;
			const int sample_per_pix;
		};

		// Samples one time per pixel
		class SimpleSampler : public Sampler {
		public:
			SimpleSampler(int xstart, int xend, int ystart, int yend);
			~SimpleSampler(){}

			bool GetSample(Sample *sample, RNG *rng);
		private:
			int x, y;
		};
	}
}