#pragma once

#include "fwddecl.h"

namespace Cruisky {
	class Shape {
	public:
		// Check if ray intersects the shape, if so fill the geometric
		// information at the intersection. Assumes that the ray 
		// has already been transformed to local space. 
		virtual bool Intersect(const Ray& ray, RayHit& geo) = 0;

		// Only checks intersection.
		virtual bool Intersect(const Ray& ray) = 0;
	};

	class UnitSphere : public Shape {
	public:
		UnitSphere(){}
		bool Intersect(const Ray& ray, RayHit& geo);
		virtual bool Intersect(const Ray& ray);
	};

	class UnitPlane : public Shape {
	public:
		UnitPlane(){}
		bool Intersect(const Ray& ray, RayHit& geo);
		virtual bool Intersect(const Ray& ray);
	};
}