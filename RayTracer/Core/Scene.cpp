#include "stdafx.h"
#include "Scene.h"
#include "PrimitiveManager.h"

namespace Cruisky {
	namespace RayTracer {

		bool Scene::Intersect(Ray& ray, Intersection& intxn) const {
			return primmgr_->Intersect(ray, intxn);
		}
		bool Scene::Occlude(const Ray& ray) const {
			return primmgr_->Occlude(ray);
		}
	}
}