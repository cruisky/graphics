#pragma once
#include "fwddecl.h"

#include <memory>
#include "Light.h"
#include "PrimitiveManager.h"

namespace TX{
	namespace RayTracer
	{
		class Scene{
		public:
			Scene(shared_ptr<Camera> camera);
			
			void AddPrimitive(shared_ptr<Primitive> prim);
			void AddLight(shared_ptr<Light> light);

			// Initializes the scene after all lights & primitives are added
			void Construct();

			bool Intersect(const Ray& ray, Intersection& intxn) const;
			void PostIntersect(const Ray& ray, LocalGeo& geo) const;
			bool Occlude(const Ray& ray) const;

		public:
			vector<shared_ptr<Light>> lights;
			shared_ptr<RayTracer::Camera> camera;
		private:
			unique_ptr<PrimitiveManager> primmgr_;
			vector<shared_ptr<Primitive>> prims_;
			
		};
	}

}