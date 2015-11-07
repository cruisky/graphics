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
		std::memcpy(buffer, ot.buffer, bufsize * sizeof(Sample));
	}

	int CameraSample::RequestSamples(int count) const {
		int start = Math::Min(offset_, bufsize - count);
		assert(start >= 0);		// buffer size is too small to allocate given number of samples in a batch
		offset_ = (offset_ + count) % bufsize;
		return start;
	}
	CameraSample::~CameraSample(){ delete[] buffer; }


	Distribution1D::Distribution1D(const float *f, uint n) : count(n) {
		func = new float[n];
		memcpy_s(func, n*sizeof(float), f, n*sizeof(float));

		// calculate cdf
		cdf = new float[n + 1];
		cdf[0] = 0.f;
		for (int i = 1; i < count + 1; i++)
			cdf[i] = cdf[i - 1] + func[i - 1];

		// normalize cdf
		funcInt = cdf[count];
		if (funcInt == 0.f) {
			float nRcp = 1.f / n;
			for (uint i = 1; i < n + 1; i++)
				cdf[i] = float(i) * nRcp;
		}
		else if (funcInt != 1.f){
			float funcIntRcp = 1.f / funcInt;
			for (uint i = 1; i < n + 1; i++)
				cdf[i] *= funcIntRcp;
		}

	}
	Distribution1D::~Distribution1D() {
		MemDeleteArray(func);
		MemDeleteArray(cdf);
	}
	float Distribution1D::SampleContinuous(float u, float *pdf, uint *off) {
		float *ptr = std::upper_bound(cdf, cdf + count, u);
		uint offset = Math::Max(0, int(ptr - cdf - 1));
		if (off) *off = offset;
		assert(offset < count);
		assert(u >= cdf[offset] && u <= cdf[offset + 1]);

		float du = (u - cdf[offset]) / (cdf[offset + 1] - cdf[offset]);
		assert(Math::Valid(du));

		if (pdf) *pdf = func[offset] / funcInt;

		return (offset + du) / count;
	}

	uint Distribution1D::SampleDiscrete(float u, float *pdf) {
		float *ptr = std::upper_bound(cdf, cdf + count, u);
		uint offset = Math::Max(0, int(ptr - cdf - 1));
		assert(offset < count);
		assert(u >= cdf[offset] && u <= cdf[offset + 1]);
		if (pdf) *pdf = func[offset] / funcInt * count;
		return offset;
	}
}
