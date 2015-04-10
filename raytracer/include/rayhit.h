#pragma once

#include "core/vector.h"

namespace Cruisky {
	namespace RayTracer {
		// Local geometry of a shape, used as the result of ray casting.
		class RayHit {
		public:
			RayHit(Vector3 point, Vector3 normal, float ray_length = Math::INF) :
				point(point), normal(normal), t(ray_length){}
			RayHit(float ray_length = Math::INF) : t(ray_length){}
			~RayHit(){}
		public:
			Vector3 point;
			Vector3 normal;
			float t;		// length of the ray, may be used in shading
		};
	}
}