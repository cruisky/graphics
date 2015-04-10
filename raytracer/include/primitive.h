#pragma once

#include "fwddecl.h"
#include "transform.h"

namespace Cruisky {
	namespace RayTracer
	{
		class Primitive {
		public:
			Primitive(Material* material) : material(material){};
			~Primitive(){};

			// TODO
			//virtual void GetBSDF(RayHit& hit, BSDF* out) const = 0;

			// Check if the global ray intersects this primitive, if so
			// set geo info and material in the intersection, 
			// and modify the length (t_max) of the ray.
			virtual bool Intersect(Ray& ray, Intersection& intersection) = 0;
			virtual bool Intersect(const Ray& ray) = 0;
		
		public:
			Transform transform;
			Material* material;
		};


		class GeometricPrimitive : public Primitive {
		public:
			GeometricPrimitive(Material* material, Shape* shape) : Primitive(material), shape(shape){};
			~GeometricPrimitive(){}

			// TODO
			//void GetBSDF(RayHit& hit, BSDF* out) const;

			bool Intersect(Ray& ray, Intersection& intersection);
			bool Intersect(const Ray& ray);
		protected:
			Shape* shape;
		};


		class AggregatePrimitive : public Primitive {
		public:
			AggregatePrimitive(Material* material) : Primitive(material) {}

			// TODO
			//void GetBSDF(RayHit& hit, BSDF* out) const;
		};
	}
}