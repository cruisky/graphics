#pragma once

#include "fwddecl.h"
#include "transform.h"

namespace Cruisky {
	namespace RayTracer
	{
		class Primitive {
		public:
			Primitive(){};

			// TODO
			//virtual void GetBSDF(RayHit& hit, BSDF* out) const = 0;

			// Check if the global ray intersects this primitive, if so
			// set geo info and material in the intersection, 
			// and modify the length (t_max) of the ray.
			virtual bool Intersect(Ray& ray, Intersection& intersection) const = 0;
			virtual bool Occlude(const Ray& ray) const = 0;
		
		public:
			Transform transform;
		};


		class GeometricPrimitive : public Primitive {
		public:
			GeometricPrimitive(const Material* material, const Shape* shape) : material(material), shape(shape){};
			~GeometricPrimitive(){}

			// TODO
			//void GetBSDF(RayHit& hit, BSDF* out) const;

			bool Intersect(Ray& ray, Intersection& intersection) const;
			bool Occlude(const Ray& ray) const;
		public:
			const Material* material;
			const Shape* shape;
		};
	}
}