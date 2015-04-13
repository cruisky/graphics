#pragma once
#include "fwddecl.h"

#include <memory>
#include "Light.h"		// for shared_ptr

namespace Cruisky{
	namespace RayTracer
	{
		class Scene{
		public:
			Scene();
			
			void AddPrimitive(Primitive * prim);
			void AddLight(Light * light);
			// Initializes the scene after all lights & primitives are added
			void Init();

			bool Intersect(const Ray& ray, Intersection& intxn) const;
			void PostIntersect(LocalGeo& geo) const;
			bool Occlude(const Ray& ray) const;

		private:
			unique_ptr<PrimitiveManager> primmgr_;
			vector<shared_ptr<Primitive>> prims_;
			vector<shared_ptr<Light>> lights_;
		};
	}

}