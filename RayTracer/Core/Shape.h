#pragma once
#include "fwddecl.h"

namespace Cruisky {
	namespace RayTracer
	{
		class Shape {
		public:
			virtual ~Shape(){};
			// Check if ray intersects the shape, if so modifies the 
			// length(t_max) of the ray. Assumes the ray is local.
			virtual bool Intersect(const Ray& localray) const = 0;
			// computes *local* intersection point, surface normal without normalizing
			virtual void PostIntersect(const Ray& localray, LocalGeo& geo) const = 0;

			// Only checks intersection.
			virtual bool Occlude(const Ray& localray) const = 0;

			virtual void Destroy(){}
		};

		class UnitSphere : public Shape {
		public:
			UnitSphere(){}
			bool Intersect(const Ray& localray) const;
			void PostIntersect(const Ray& localray, LocalGeo& geo) const;
			bool Occlude(const Ray& localray) const;
		};

		class UnitPlane : public Shape {
		public:
			UnitPlane(){}
			bool Intersect(const Ray& localray) const;
			void PostIntersect(const Ray& localray, LocalGeo& geo) const;
			bool Occlude(const Ray& localray) const;
		};
	}
}