#pragma once

#include "Util.h"
#include "Math/Vector.h"

namespace TX{

	class Ray{
	public:
		static const float EPSILON;
	public:
		Ray() : t_min(EPSILON), t_max(Math::INF){}
		Ray(const Vector3& origin, const Vector3& vdir, float t_max = Math::INF, float t_min = EPSILON) :
			origin(origin), dir(vdir), t_min(t_min), t_max(t_max){ dir.Normalize(); }
		Ray(const Ray& ot) :
			origin(ot.origin), dir(ot.dir), t_min(ot.t_min), t_max(ot.t_max){}
		~Ray(){}

		Ray& operator = (const Ray& ot);
		bool operator ==(const Ray& ot);

		// Reset the value of this ray, without normalizing direction vector.
		inline Ray& Reset(const Vector3& origin = Vector3::ZERO, const Vector3& dir = Vector3::Z, float t_max = Math::INF, float t_min = EPSILON){
			this->t_min = t_min;
			this->t_max = t_max;
			this->origin = origin;
			this->dir = dir;
			return *this;
		}

		// Sets origin,dir,t values of this ray using two points, and normalize it.
		inline Ray& SetSegment(const Vector3& orig, const Vector3& dest, float eps_dest = EPSILON, float eps_origin = EPSILON){
			origin = orig;
			dir = dest - orig;
			t_min = eps_origin;
			t_max = dir.Length();
			dir /= t_max;
			t_max -= eps_dest;
			return *this;
		}

		// The target point
		inline Vector3 End() const { return origin + t_max * dir; }
	public:
		Vector3 origin;
		Vector3 dir;
		mutable float t_min, t_max;
	};

	inline std::ostream& operator << (std::ostream& os, const Ray& r){
		return os << "[" << r.origin << " -> " << r.dir << ", " << "(" << r.t_min << "~" << r.t_max << ")]";
	}

	inline Ray& Ray::operator = (const Ray& ot){
		origin = ot.origin;
		dir = ot.dir;
		t_min = ot.t_min;
		t_max = ot.t_max;
		return *this;
	}

	inline bool Ray::operator == (const Ray& ot){
		return origin == ot.origin
			&& dir == ot.dir
			&& t_min == ot.t_min
			&& t_max == ot.t_max;
	}
}