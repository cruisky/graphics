#pragma once

#include "fwddecl.h"
#include "stdafx.h"

namespace Cruisky{
	namespace RayTracer
	{
		class Scene{
		public:
			Scene();

			void AddPrimitive(const shared_ptr<Primitive>& prim);
			void AddLight(const Light& light);
			bool Intersect(Ray& ray, Intersection& intersection) const;
			bool Occlude(const Ray& ray) const;

		private:
			unique_ptr<PrimitiveManager> primmgr_;
			vector<shared_ptr<Primitive>> prims_;
			vector<shared_ptr<Light>> lights_;
		};
	}

}