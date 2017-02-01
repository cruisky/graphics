#pragma once
#include "txbase/fwddecl.h"
#include "txbase/math/vector.h"

namespace TX {
	class BBox {
	public:
		union {
			struct{ Vec3 min, max; };
			Vec3 bounds[2];
		};

	public:
		BBox() : min(Math::INF), max(-Math::INF) {}
		BBox(const BBox& ot) : min(ot.min), max(ot.max){}
		BBox(const Vec3& p) : min(p), max(p) {}
		BBox(const Vec3& p1, const Vec3& p2) :
			min(Math::Min(p1, p2)),
			max(Math::Max(p1, p2)){}

		inline BBox& operator = (const BBox& ot) { min = ot.min; max = ot.max; return *this; }
		inline const Vec3& operator [] (size_t idx) const { assert(idx < 2); return bounds[idx]; }
		inline		 Vec3& operator [] (size_t idx)		  { assert(idx < 2); return bounds[idx]; }

		inline bool operator == (const BBox& ot) const { return min == ot.min && max == ot.max; }
		inline bool operator != (const BBox& ot) const { return min != ot.min || max != ot.max; }

		inline float Area() const { Vec3 dim = max - min; return 2.f * (dim.x * dim.y + dim.y * dim.z + dim.z * dim.x); }
		inline float Volume() const { Vec3 dim = max - min; return dim.x * dim.y * dim.z; }
		inline Vec3 Centroid() const { return 0.5f * (min + max); }
		inline void BoundingSphere(Vec3 *center, float *radius) const { *center = Centroid(); *radius = Inside(*center) ? Math::Dist(max, *center) : 0.f; }
		inline bool Overlaps(const BBox& ot) const { return max >= ot.min && min <= ot.max; }
		inline bool Inside(const Vec3& p) const { return p >= min && p <= max; }
		inline Vec3 Offset(const Vec3& p) const { return (p - min) /= (max - min); }
		inline void Expand(float delta) { Vec3 d(delta); min -= d; max += d; }
		inline int MaximumExtent() const {
			Vec3 dim = max - min;
			return
				(dim.x < dim.y && dim.x > dim.z) ? 0 :
				(dim.y > dim.z) ? 1 :
				2;
		}
	};

	namespace Math {
		inline BBox Union(const BBox& box1, const BBox& box2) {
			BBox ret;
			ret.min = Math::Min(box1.min, box2.min);
			ret.max = Math::Max(box1.max, box2.max);
			return ret;
		}
		inline BBox Union(const BBox& box, const Vec3& p) {
			BBox ret;
			ret.min = Math::Min(box.min, p);
			ret.max = Math::Max(box.max, p);
			return ret;
		}
	}
}
