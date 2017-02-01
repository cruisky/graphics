#pragma once
#include "txbase/fwddecl.h"

namespace TX {
	class Shape {
	public:
		virtual ~Shape(){};
		/// <summary>
		/// Surface area.
		/// </summary>
		virtual float Area() const = 0;
		/// <summary>
		/// General sampling.
		/// </summary>
		virtual void SamplePoint(const Sample *sample, Vec3 *out, Vec3 *normal) const = 0;
		/// <summary>
		/// Samples only from points that are visible from the eye.
		/// </summary>
		virtual void SamplePoint(const Sample *sample, const Vec3& localeye, Vec3 *out, Vec3 *normal) const {
			SamplePoint(sample, out, normal);
		}
	};
}
