#pragma once
#include "fwddecl.h"

#include "stdafx.h"

namespace TX
{
	namespace RayTracer
	{
		class PrimitiveManager {
		public:
			PrimitiveManager(vector<shared_ptr<Primitive>> *prims){
				prims_ = prims;
			}
			virtual ~PrimitiveManager(){}

			virtual void Construct(){}

			virtual bool Intersect(const Ray& ray, Intersection& intxn) const;
			virtual bool Occlude(const Ray& ray) const;
		
		public:
			vector<shared_ptr<Primitive>> *prims_;
		};
	}
}