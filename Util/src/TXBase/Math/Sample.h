#pragma once
#include <iostream>
#include "txbase/math/vector.h"
#include "txbase/math/random.h"
#include "txbase/sys/memory.h"

namespace TX{
	class Sample {
	public:
		float u, v, w;
		Sample():u(0.5f), v(0.5f), w(0.5f){}
		Sample(RNG& rng);
		/// <summary> All values are within range [0, 1]. </summary>
		Sample(float u, float v, float w);
	};
	std::ostream& operator<<(std::ostream& os, const Sample& sample);

	/// <summary>
	/// A sample buffer containing all samples for tracing a single ray.
	/// </summary>
	class CameraSample {
	public:
		/// <summary>
		/// Buffer size should be at least the maximum number of samples being requested every time.
		/// </summary>
		CameraSample(int bufsize);
		CameraSample(const CameraSample& ot);
		/// <summary>
		/// Returns a pointer to an array of at least <paramref name="count"/> samples
		/// </summary>
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

	/// <summary>
	/// Pointer to a sample array.
	/// </summary>
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


	class Distribution1D {
	public:
		float *func, *cdf;		// samples and cumulative distro function
		float funcInt;			// integral
		int count;
	public:
		/// <summary>
		/// Constructor of Distribution1D.
		/// </summary>
		/// <param name="f"> An array of samples representing the distribution function </param>
		/// <param name="n"> Number of samples </param>
		Distribution1D(const float *f, uint n);
		~Distribution1D();
		/// <summary>
		/// Take sample as continuous distribution.
		/// </summary>
		/// <param name="u"> Random sample </param>
		/// <param name="pdf"> Pdf at the sampled point </param>
		/// <param name="off"> Offset of the section where sample is taken from </param>
		/// <returns> [0, 1) </returns>
		float SampleContinuous(float u, float *pdf, uint *off = nullptr);
		/// <summary>
		/// Take sample as discrete distribution.
		/// </summary>
		/// <param name="u"> Random sample </param>
		/// <param name="pdf"> Pdf at the sample point </param>
		/// <returns> The sample offset </returns>
		uint SampleDiscrete(float u, float *pdf);
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
