#include "stdafx.h"

#include "Sample.h"
#include "Vector.h"
#include "RNG.h"
#include "MathUtil.h"

namespace Cruisky{
	namespace RayTracer{
		Sample::Sample(RNG& rng){ u = rng.Float(); v = rng.Float(); w = rng.Float(); }
		Sample::Sample(float u, float v, float w) :
			u(u), v(v), w(w){}

		std::ostream& operator<<(std::ostream& os, const Sample& sample){
			os << "(" << sample.u << "," << sample.v << ")";
			return os;
		}


		CameraSample::CameraSample(int bufsize) : offset_(0), bufsize(bufsize), buffer(new Sample[bufsize]){}
		Sample * CameraSample::RequestSamples(int count) const {
			int start = Math::Min(offset_, bufsize - count);
			assert(start >= 0);		// buffer size is too small to allocate given number of samples in a batch
			offset_ = (offset_ + count) % bufsize;
			return buffer + start;
		}
		CameraSample::~CameraSample(){ delete[] buffer; }
	}
}