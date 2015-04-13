#pragma once

#include "fwddecl.h"
#include "Transform.h"
#include "BSDF.h"		// shared_ptr
#include "Shape.h"		// shared_ptr

namespace Cruisky {
	namespace RayTracer
	{
		class Primitive {
		public:
			Primitive(const BSDF *bsdf, const Shape *shape) : bsdf_(bsdf), shape_(shape){};

			// Checks if the global ray intersects this primitive, if so
			// fills dist and prim in the intersection, 
			// and modify the length (t_max) of the ray.
			bool Intersect(const Ray& ray, Intersection& intersection) const;
			void PostIntersect(LocalGeo& geo) const;
			bool Occlude(const Ray& ray) const;

			const BSDF* GetBSDF() const;
		public:
			Transform transform;
		private:
			shared_ptr<const BSDF>	bsdf_;
			shared_ptr<const Shape>	shape_;
			mutable Ray localray_;			// last local ray
		};
	}
}