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
			assert(start >= 0);
			offset_ = (offset_ + count) % bufsize;
			return buffer + start;
		}
		CameraSample::~CameraSample(){ delete[] buffer; }

		namespace MonteCarlo{
			Vector3 UniformHemisphere(float u1, float u2){
				float r = Math::Sqrt(Math::Max(0.f, 1.f - u1 * u1));		// radius [0, 1]
				float phi = Math::TWO_PI * u2;								// angle
				return Vector3(r * Math::Cos(phi), r * Math::Sin(phi), u1);
			}

			Vector3 UniformSphere(float u1, float u2){
				float z = 1.f - 2.f * u1;									// radius [-1, 1]
				float r = Math::Sqrt(Math::Max(0.f, 1.f - z * z));
				float phi = Math::TWO_PI * u2;
				return Vector3(r * Math::Cos(phi), r * Math::Sin(phi), z);
			}

			void UniformDisk(float u1, float u2, float *x, float *y){
				float r = Math::Sqrt(u1);
				float theta = Math::TWO_PI * u2;
				*x = r * Math::Cos(theta);
				*y = r * Math::Sin(theta);
			}

			void UniformTriangle(float u1, float u2, float *u, float *v){
				float su1 = Math::Sqrt(u1);
				*u = 1.f - su1;
				*v = u2 * su1;
			}

			void ConcentricDisk(float u1, float u2, float *u, float *v){
				float r, theta;
				// Map to [-1, 1]
				float sx = 2.f * u1 - 1.f;
				float sy = 2.f * u2 - 1.f;

				if (sx == 0.f && sy == 0.f){
					*u = 0.f;
					*v = 0.f;
					return;
				}
				if (sx >= -sy){
					if (sx > sy){
						// 1
						r = sx;
						if (sy > 0.f) theta = sy / r;
						else theta = 8.f + sy / r;
					}
					else {
						// 2
						r = sy;
						theta = 2.f - sx / r;
					}
				}
				else {
					if (sx <= sy){
						// 3
						r = -sx;
						theta = 4.f - sy / r;
					}
					else {
						// 4
						r = -sy;
						theta = 6.f + sx / r;
					}
				}
				theta *= Math::FOUR_PI;
				*u = r * Math::Cos(theta);
				*v = r * Math::Sin(theta);
			}
		}
	}
}