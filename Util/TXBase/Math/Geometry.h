#pragma once

#include <vector>
#include "TXBase/fwddecl.h"
#include "Vector.h"

namespace TX
{
	struct Rect {
		union{
			struct { Vec2 min, max; };
			float f[4];
		};
		Rect() : min(), max(){}
		~Rect(){}
		Rect(const Rect& ot) : min(ot.min), max(ot.max) {}
		Rect(float x1, float y1, float x2, float y2) : min(x1, y1), max(x2, y2) {}
		Rect(const Vec2& min, const Vec2& max) : min(min), max(max){}
		Rect(const Vec4& v) : min(v.x, v.y), max(v.z, v.w){}

		inline Rect& operator = (const Rect& ot) { min = ot.min; max = ot.max; return *this; }

		inline bool Valid() const { return min <= max; }
		inline Vec2 Center() const { return (min + max) * 0.5f; }
		inline Vec2 Size() const { return max - min; }
		inline float Width() const { return max.x - min.x; }
		inline float Height() const { return max.y - min.y; }
		inline Vec2 TL() const { return min; }
		inline Vec2 TR() const { return Vec2(max.x, min.y); }
		inline Vec2 BL() const { return Vec2(min.x, max.y); }
		inline Vec2 BR() const { return max; }
		inline bool Contains(const Vec2& p) const { return Math::InBounds(p.x, min.x, max.x) && Math::InBounds(p.y, min.y, max.y); }
		inline bool Contains(const Rect& r) const { return r.min >= min && r.max < max; };
		inline bool Overlaps(const Rect& r) const { return r.min < max && r.max > min; }
		inline Rect& Move(const Vec2& offset) { min += offset; max += offset; return *this; }
		inline Rect& MoveTo(const Vec2& pos) { Move(pos - min); return *this; }
		inline Rect& Expand(const Vec2& border) { if (-border < Size()){ min -= border; max += border; } return *this; }
		inline Rect& Shrink(const Vec2& border) { if (border < Size()){ min += border; max -= border; } return *this; }
		inline Rect& Add(const Vec2& p) { min = Math::Min(min, p); max = Math::Max(max, p); return *this; }
		inline Rect& Add(const Rect& r) { min = Math::Min(min, r.min); max = Math::Max(max, r.max); return *this; }
		inline Rect& Round() { min.x = (float)(int)min.x; min.y = (float)(int)min.y; max.x = (float)(int)max.x; max.y = (float)(int)max.y; return *this; }
		inline Rect& Clip(const Rect& clip) {
			bool overlapping = Overlaps(clip);
			min = overlapping ? Math::Max(min, clip.min) : Vec2(0.f);
			max = overlapping ? Math::Min(max, clip.max) : Vec2(0.f);
			return *this;
		}
		inline Vec2 ClosestPoint(const Vec2& p, bool onEdge = true) const {
			if (!onEdge && Contains(p)) return p;
			return Math::Max(Math::Min(p, max), min);
		}
	};

	inline std::ostream& operator << (std::ostream& os, const Rect& rect){
		os << rect.min << " - " << rect.max;
	}

	/// <summary>
	/// Assumes vertices are CCW ordered.
	/// </summary>
	class Polygon {
		struct Vertex {
			int id;
			bool active;
			bool ear;
			bool convex;
			const Vec2 *p;
			Vertex *next;
			Vertex *prev;
			float cosine;
			Vertex() : active(true){}
		};
	public:
		std::vector<Vec2> points;
	public:
		Polygon(){}
		Polygon(const Vec2 *pts, int count) : points(pts, pts + count){}
		~Polygon(){}
	public:
		static bool IsConvex(const Vec2& v1, const Vec2& v2, const Vec2& v3);
	public:
		inline const Vec2& operator [] (size_t i) const { return points[i]; }
		inline       Vec2& operator [] (size_t i)       { return points[i]; }
		inline size_t Count() const { return points.size(); }
		inline void Push(const Vec2& p){ points.push_back(p); }
		inline void Pop(){ points.pop_back(); }
		inline void Clear(){ points.clear(); }
		void Triangulate(uint16 *indexBuffer, uint16 idxOffset = 0) const;
	private:
		static void UpdateVertex(Vertex& v, Vertex *vertices, int count);
	};

}
