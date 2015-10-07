#include "UtilStdAfx.h"
#include "Sample.h"

namespace TX{
	Sample::Sample(RNG& rng){ u = rng.Float(); v = rng.Float(); w = rng.Float(); }
	Sample::Sample(float u, float v, float w) :
		u(u), v(v), w(w){}

	std::ostream& operator<<(std::ostream& os, const Sample& sample){
		os << "(" << sample.u << "," << sample.v << ")";
		return os;
	}


	CameraSample::CameraSample(int bufsize) : offset_(0), bufsize(bufsize), buffer(new Sample[bufsize]){}
	CameraSample::CameraSample(const CameraSample& ot)
		: offset_(0), bufsize(ot.bufsize), buffer(new Sample[ot.bufsize]){
		memcpy_s(buffer, bufsize * sizeof(Sample), ot.buffer, bufsize * sizeof(Sample));
	}

	int CameraSample::RequestSamples(int count) const {
		int start = Math::Min(offset_, bufsize - count);
		assert(start >= 0);		// buffer size is too small to allocate given number of samples in a batch
		offset_ = (offset_ + count) % bufsize;
		return start;
	}
	CameraSample::~CameraSample(){ delete[] buffer; }
}
