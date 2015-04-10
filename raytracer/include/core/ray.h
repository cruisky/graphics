#pragma once

#include "fwddecl.h"
#include "vector.h"

namespace Cruisky{

	class Ray{
	public:
		static const float EPSILON;
	public:
		Ray();
		Ray(const Vector3& pOrigin, const Vector3& vDir, float t_min = EPSILON, float t_max = Math::INF) :
			origin(pOrigin), dir(vDir), t_min(t_min), t_max(t_max){ dir.Normalize(); }
		Ray(const Ray& ot) :
			origin(ot.origin), dir(ot.dir), t_min(ot.t_min), t_max(ot.t_max){}
		~Ray(){}
	public:
		Vector3 origin;
		Vector3 dir;
		float t_min;
		float t_max;
	};
}