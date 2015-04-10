#pragma once

#include "fwddecl.h"

namespace Cruisky {
	namespace RayTracer
	{
		class Shape {
		public:
			// Check if ray intersects the shape, if so fill the geometric
			// information at the hit. Assumes the ray is local.
			virtual bool Intersect(Ray& localray, RayHit& hit) const = 0;

			// Only checks intersection.
			virtual bool Occlude(Ray& localray) const = 0;
		};

		class UnitSphere : public Shape {
		public:
			UnitSphere(){}
			bool Intersect(Ray& localray, RayHit& hit) const;
			bool Occlude(Ray& localray) const;
		};

		class UnitPlane : public Shape {
		public:
			UnitPlane(){}
			bool Intersect(Ray& localray, RayHit& hit) const;
			bool Occlude(Ray& localray) const;
		};
	}
}