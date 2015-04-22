#pragma once

#include "../Util.h"
#include "Vector.h"

namespace Cruisky{

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

		inline Ray& Reset(){
			t_min = EPSILON;
			t_max = Math::INF;
			origin = Vector3::ZERO;
			dir = Vector3(0.f, 0.f, -1.f);
			return *this;
		}

		inline Ray& SetSegment(const Vector3& orig, const Vector3& dest){
			origin = orig;
			dir = dest - orig;
			t_min = Ray::EPSILON;
			t_max = Length(dir);
			dir /= t_max;
			return *this;
		}
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
}