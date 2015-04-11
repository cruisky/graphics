#pragma once

#include "vector.h"

namespace Cruisky {
	namespace RayTracer {
		// Local geometry of a shape, used as the result of ray casting.
		class RayHit {
		public:
			RayHit(){}
			RayHit(Vector3 point, Vector3 normal) :
				point(point), normal(normal){}
			~RayHit(){}

		public:
			Vector3 point;
			Vector3 normal;
		};
	}
}