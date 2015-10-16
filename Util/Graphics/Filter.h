#pragma once

#include "Math/Base.h"

namespace TX
{
	class Filter{
	public:
		const float radius;
	public:
		Filter(const float radius) : radius(radius){}
		virtual ~Filter(){}

		virtual const float Eval(const float dx, const float dy) const = 0;
	};


	class BoxFilter : public Filter{
	public:
		BoxFilter() : Filter(0.5f){}
	public:
		const float Eval(const float dx, const float dy) const { return 1.f; }
	};

	class GaussianFilter : public Filter{
	private:
		float sd;
		float alpha;
		float exp_r;
	public:
		GaussianFilter(const float sd = 0.5f)
			: Filter(4 * sd), sd(sd){
			alpha = -1.f / (2.f * sd * sd);
			exp_r = Math::Exp(alpha * radius * radius);
		}
		const float Eval(const float dx, const float dy) const {
			return Gaussian1D(dx) * Gaussian1D(dy);
		}
	private:
		inline float Gaussian1D(const float d) const {
			return Math::Max(0.f, Math::Exp(alpha * d * d) - exp_r);
		}
	};
}