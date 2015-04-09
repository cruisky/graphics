#pragma once

#include "vector.h"
#include "color.h"

namespace Cruisky{
	class Ray{
	public:
		static const float EPSILON;
		Vector3 origin;
		Vector3 dir;
		Color color;
	public:
		Ray();
		Ray(Vector3 pOrigin, Vector3 vDir) : origin(pOrigin), dir(vDir){
			dir.Normalize();
		}
	};

	const float Ray::EPSILON = 1e-6f;
}