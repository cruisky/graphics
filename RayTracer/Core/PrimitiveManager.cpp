#include "stdafx.h"

#include "PrimitiveManager.h"
#include "Primitive.h"
namespace Cruisky
{
	namespace RayTracer
	{
		bool PrimitiveManager::Intersect(const Ray& ray, Intersection& intxn) const{
			bool hasIntxn = false;
			for (auto p : *prims_){
				if (p->Intersect(ray, intxn) && !hasIntxn)
					hasIntxn = true;
			}
			return hasIntxn;
		}

		bool PrimitiveManager::Occlude(const Ray& ray) const{
			for (auto p : *prims_){
				if (p->Occlude(ray))
					return true;
			}
			return false;
		}
	}
}