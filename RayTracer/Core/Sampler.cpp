#include "stdafx.h"

#include "Sampler.h"

namespace Cruisky{
	namespace RayTracer {
		bool Sampler::GetSample(Sample *sample){
			sample->u = sample->v = 0.5f;
		}
	}
}