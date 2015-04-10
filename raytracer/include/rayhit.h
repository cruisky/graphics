#pragma once

#include "core/vector.h"

namespace Cruisky {
	namespace RayTracer {
		// Local geometry of a shape, used as the result of ray casting.
		class RayHit {
		public:
			RayHit(float tmax = 0.f) : t(tmax){}
			RayHit(Vector3 point, Vector3 normal) :
				point(point), normal(normal){}
			~RayHit(){}
		public:
			Vector3 point;
			Vector3 normal;
			float t;		// length of the ray
		};
	}
}