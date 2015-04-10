#pragma once

#include "fwddecl.h"

namespace Cruisky {
	namespace RayTracer
	{
		class Shape {
		public:
			// Check if ray intersects the shape, if so fill the geometric
			// information at the hit. Assumes the ray is local.
			virtual bool Intersect(const Ray& localray, RayHit& hit) = 0;

			// Only checks intersection.
			virtual bool Intersect(const Ray& localray) = 0;
		};

		class UnitSphere : public Shape {
		public:
			UnitSphere(){}
			bool Intersect(const Ray& localray, RayHit& hit);
			virtual bool Intersect(const Ray& localray);
		};

		class UnitPlane : public Shape {
		public:
			UnitPlane(){}
			bool Intersect(const Ray& localray, RayHit& hit);
			virtual bool Intersect(const Ray& localray);
		};
	}
}