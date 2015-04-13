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

			virtual void Construct(const vector<shared_ptr<Primitive>>& prims);

			virtual bool Intersect(const Ray& ray, Intersection& intxn) const;
			virtual bool Occlude(const Ray& ray) const;
		
		public:
			vector<shared_ptr<Primitive>> *prims_;
		};
	}
}