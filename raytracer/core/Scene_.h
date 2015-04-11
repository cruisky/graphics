#pragma once

#include "fwddecl.h"
#include "stdafx.h"

namespace Cruisky{
	namespace RayTracer
	{
		class Scene{
		public:
			Scene();

			inline bool Intersect(Ray& ray, Intersection& intersection) const;
			inline bool Occlude(const Ray& ray) const;

		private:
			unique_ptr<PrimitiveManager> primmgr_;
			vector<shared_ptr<Primitive>> prims_;
			vector<shared_ptr<Light>> lights_;
		};
	}

}