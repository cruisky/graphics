#pragma once 

#include "fwddecl.h"
#include <iostream>
#include "Vector.h"

namespace Cruisky{
	namespace RayTracer{
		struct Sample {
			float u, v, w; 
			Sample():u(0.5f), v(0.5f), w(0.5f){}
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

		namespace MonteCarlo {
			Vector3 UniformHemisphere(float u1, float u2);
			Vector3 UniformSphere(float u1, float u2);
			void UniformDisk(float u1, float u2, float *x, float *y);
			void UniformTriangle(float u1, float u2, float *u, float *v);
			void ConcentricDisk(float u1, float u2, float *u, float *v);
			inline Vector3 CosineHemisphere(float u1, float u2){
				Vector3 ret;
				ConcentricDisk(u1, u2, &ret.x, &ret.y);
				ret.z = Math::Sqrt(Math::Max(0.f, 1.f - ret.x * ret.x - ret.y * ret.y));
				return ret;
			}


			inline float UniformHemispherePdf(){ return Math::TWO_PI_INV; }
			inline float UniformSpherePdf(){ return Math::FOUR_PI_INV; }
			inline float CosineHemispherePdf(float costheta, float phi){
				return costheta * Math::PI_INV;
			}
		}
	}
}