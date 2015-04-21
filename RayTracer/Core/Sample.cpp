#include "stdafx.h"

#include "Sample.h"
#include "RNG.h"

namespace Cruisky{
	namespace RayTracer{
		Sample::Sample(RNG& rng){ u = rng.Float(); v = rng.Float(); w = rng.Float(); }
		Sample::Sample(float u, float v, float w) :
			u(u), v(v), w(w){}

		std::ostream& operator<<(std::ostream& os, const Sample& sample){
			os << "(" << sample.u << "," << sample.v << ")";
			return os;
		}


		CameraSample::CameraSample(int bufsize) : bufsize(bufsize), buffer(new Sample[bufsize]){}
		CameraSample::~CameraSample(){ delete[] buffer; }
	}
}