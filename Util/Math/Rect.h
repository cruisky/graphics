#pragma once

#include "Vector.h"

namespace TX
{
	struct Rect {
		Vector2 min, max;
		Rect(){}
		Rect(const Vector2& min, const Vector2& max) : min(min), max(max){}
		Rect(const Vector4& v) : min(v.x, v.y), max(v.z, v.w){}
		Rect(float x1, float y1, float x2, float y2) : min(x1, y1), max(x2, y2){}

		inline Vector2 Center() const { return (min + max) * 0.5f; }
		inline Vector2 Size() const { return max - min; }
		inline float Width() const { return max.x - min.x; }
		inline float Height() const { return max.y - min.y; }
		inline Vector2 TL() const { return min; }
		inline Vector2 TR() const { return Vector2(max.x, min.y); }
		inline Vector2 BL() const { return Vector2(min.x, max.y); }
		inline Vector2 BR() const { return max; }
		inline bool Contains(const Vector2& p) const { return Math::InBounds(p.x, min.x, max.x) && Math::InBounds(p.y, min.y, max.y); }
		inline bool Contains(const Rect& r) const { return r.min >= min && r.max < max; };
		inline bool Overlaps(const Rect& r) const { return r.min < max && r.max > min; }
		inline Rect& Move(const Vector2& offset) { min += offset; max += offset; return *this; }
		inline Rect& MoveTo(const Vector2& pos) { Move(pos - min); return *this; }
		inline Rect& Expand(const Vector2& border) { if (-border < Size()){ min -= border; max += border; } return *this; }
		inline Rect& Shrink(const Vector2& border) { if (border < Size()){ min += border; max -= border; } return *this; }
		inline Rect& Add(const Vector2& p) { min = Math::Min(min, p); max = Math::Max(max, p); return *this; }
		inline Rect& Add(const Rect& r) { min = Math::Min(min, r.min); max = Math::Max(max, r.max); return *this; }
		inline Rect& Round() { min.x = (float)(int)min.x; min.y = (float)(int)min.y; max.x = (float)(int)max.x; max.y = (float)(int)max.y; return *this; }
		inline Rect& Clip(const Rect& clip) { min = Math::Max(min, clip.min); max = Math::Min(max, clip.max); return *this; }
		inline Vector2 ClosestPoint(const Vector2& p, bool onEdge = true) const {
			if (!onEdge && Contains(p)) return p;
			return Math::Max(Math::Min(p, max), min);
		}
	};

	inline std::ostream& operator << (std::ostream& os, const Rect& rect){
		os << rect.min << " - " << rect.max;
	}
}
