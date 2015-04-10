#pragma once

#include "fwddecl.h"
#include "transform.h"

namespace Cruisky {
	namespace RayTracer
	{
		class Primitive{
		public:
			Primitive(Material* material) : material(material){};
			~Primitive(){};

			Material* GetMaterial(){ return material; }

			// Check if the global ray intersects this primitive, if so
			// set geo info and material in the intersection, and return true.
			virtual bool Intersect(const Ray& ray, Intersection& intersection) = 0;
			virtual bool Intersect(const Ray& ray) = 0;
		protected:
			Transform transform;
			Material* material;
		};


		class GeometricPrimitive : protected Primitive {
		public:
			GeometricPrimitive(Material* material, Shape* shape) :Primitive(material), shape(shape){};
			~GeometricPrimitive(){}

			bool Intersect(const Ray& ray, Intersection& intersection);
			bool Intersect(const Ray& ray);
		protected:
			Shape* shape;
		};
	}
}