#pragma once

#include "fwddecl.h"
#include "stdafx.h"

namespace Cruisky
{
	namespace RayTracer
	{
		class PrimitiveManager {
		public:
			PrimitiveManager(){}

			virtual void Init(vector<shared_ptr<Primitive>>& prims) = 0;

			virtual bool Intersect(Ray& ray, Intersection& intxn) const = 0;
			virtual bool Occlude(const Ray& ray) const;
		};
	}
}