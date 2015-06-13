#pragma once
#include "fwddecl.h"

#include "SceneObject.h"
#include <memory>
#include "BSDF.h"		// shared_ptr
#include "Shape.h"		// shared_ptr

namespace Cruisky {
	namespace RayTracer
	{
		class Primitive : public SceneObject {
		public:
			Primitive(std::shared_ptr<const Shape> shape, std::shared_ptr<const BSDF> bsdf) : bsdf_(bsdf), shape_(shape){};
			virtual ~Primitive(){}

			// Checks if the global ray intersects this primitive, if so
			// fills dist and prim in the intersection, 
			// and modify the length (t_max) of the ray.
			bool Intersect(const Ray& ray, Intersection& intersection) const;
			// Extracts info for the LocalGeo.
			void PostIntersect(const Ray& ray, LocalGeo& geo) const;
			// Test if this object occludes the ray.
			bool Occlude(const Ray& ray) const;

			const BSDF* GetBSDF() const;
		private:
			std::shared_ptr<const BSDF>		bsdf_;
			std::shared_ptr<const Shape>	shape_;
			mutable Ray localray_;		// last local ray
		};
	}
}