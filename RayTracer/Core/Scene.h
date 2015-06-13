#pragma once
#include "fwddecl.h"

#include <memory>
#include "Light.h"
#include "PrimitiveManager.h"

namespace Cruisky{
	namespace RayTracer
	{
		class Scene{
		public:
			Scene();
			
			void AddPrimitive(shared_ptr<Primitive> prim);
			void AddLight(shared_ptr<Light> light);

			// Initializes the scene after all lights & primitives are added
			void Construct();

			bool Intersect(const Ray& ray, Intersection& intxn) const;
			void PostIntersect(const Ray& ray, LocalGeo& geo) const;
			bool Occlude(const Ray& ray) const;

		public:
			vector<shared_ptr<Light>> lights;
		private:
			unique_ptr<PrimitiveManager> primmgr_;
			vector<shared_ptr<Primitive>> prims_;
			
		};
	}

}