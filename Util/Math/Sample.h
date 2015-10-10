#pragma once
#include <iostream>
#include "Math/Vector.h"
#include "Math/Random.h"

namespace TX{
	class Sample {
	public:
		float u, v, w;
		Sample():u(0.5f), v(0.5f), w(0.5f){}
		Sample(RNG& rng);
		// all values are within range [0, 1]
		Sample(float u, float v, float w);
	};
	std::ostream& operator<<(std::ostream& os, const Sample& sample);

	// A sample buffer containing all samples for tracing a single ray.
	class CameraSample {
	public:
		// Buffer size should at least the maximum number of samples being requested
		CameraSample(int bufsize);
		CameraSample(const CameraSample& ot);
		// Returns a pointer to an array of at least <count> samples
		int RequestSamples(int count) const;
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

	// Just an integer
	class SampleOffset {
	public:
		SampleOffset() : offset(0){}
		SampleOffset(const SampleOffset& ot) : offset(ot.offset){}
		SampleOffset(int count, const CameraSample *sample_buf){
			RequestSamples(count, sample_buf);
		}
		void RequestSamples(int count, const CameraSample *sample_buf){
			offset = sample_buf->RequestSamples(count);
		}
		const Sample * operator()(const CameraSample& sample_buf){
			assert(offset < sample_buf.bufsize);
			return sample_buf.buffer + offset;
		}
	private:
		int offset;
	};

	namespace Sampling {
		inline Vec3 UniformHemisphere(float u1, float u2){
			float r = Math::Sqrt(Math::Max(0.f, 1.f - u1 * u1));		// radius [0, 1]
			float phi = 2.f * Math::PI * u2;								// angle
			return Vec3(r * Math::Cos(phi), r * Math::Sin(phi), u1);
		}
		inline Vec3 UniformSphere(float u1, float u2){
			float z = 1.f - 2.f * u1;									// radius [-1, 1]
			float r = Math::Sqrt(Math::Max(0.f, 1.f - z * z));
			float phi = 2.f * Math::PI * u2;
			return Vec3(r * Math::Cos(phi), r * Math::Sin(phi), z);
		}
		inline Vec3 UniformCone(float u1, float u2, float max_costheta){
			float costheta = Math::Lerp(max_costheta, 1.0f, u1);
			float sintheta = sqrtf(1.f - costheta * costheta);
			float phi = 2.f * Math::PI * u2;
			return Vec3(Math::Cos(phi) * sintheta, Math::Sin(phi) * sintheta, costheta);
		}
		inline void UniformDisk(float u1, float u2, float *x, float *y){
			float r = Math::Sqrt(u1);
			float theta = 2.f * Math::PI * u2;
			*x = r * Math::Cos(theta);
			*y = r * Math::Sin(theta);
		}
		inline void UniformTriangle(float u1, float u2, float *u, float *v){
			float su1 = Math::Sqrt(u1);
			*u = 1.f - su1;
			*v = u2 * su1;
		}
		inline void ConcentricDisk(float u1, float u2, float *u, float *v){
			// http://psgraphics.blogspot.ch/2011/01/improved-code-for-concentric-map.html
			float a = 2.f * u1 - 1.f;
			float b = 2.f * u2 - 1.f;
			float phi, r;
			if (a == 0.f && b == 0.f){
				*u = *v = 0.f;
				return;
			}
			if (a*a > b*b)	// use squares instead of absolute values
			{
				r = a;
				phi = Math::PI / 4.f * (b / a);
			}
			else {
				r = b;
				phi = Math::PI / 4.f * (a / b) + Math::PI / 2.f;
			}
			*u = r * Math::Cos(phi);
			*v = r * Math::Sin(phi);
		}
		inline Vec3 CosineHemisphere(float u1, float u2){
			Vec3 ret;
			ConcentricDisk(u1, u2, &ret.x, &ret.y);
			ret.z = Math::Sqrt(Math::Max(0.f, 1.f - ret.x * ret.x - ret.y * ret.y));
			return ret;
		}
		inline float UniformHemispherePdf(){ return Math::PI_RCP * 0.5f; }
		inline float UniformSpherePdf(){ return Math::PI_RCP * 0.25f; }
		inline float CosineHemispherePdf(float costheta, float phi){
			return costheta * Math::PI_RCP;
		}
	}
}
