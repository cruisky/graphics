#include "stdafx.h"
#include "Scene.h"
#include "PrimitiveManager.h"
#include "Primitive.h"
#include "Intersection.h"

namespace Cruisky {
	namespace RayTracer {
		void Scene::AddPrimitive(Primitive * prim){
			prims_.push_back(shared_ptr<Primitive>(prim));
		}
		void Scene::AddLight(Light * light){
			lights_.push_back(shared_ptr<Light>(light));
		}
		void Scene::Init(){
			primmgr_ = unique_ptr<PrimitiveManager>(new PrimitiveManager());
		}
		bool Scene::Intersect(const Ray& ray, Intersection& intxn) const {
			return primmgr_->Intersect(ray, intxn);
		}
		void Scene::PostIntersect(LocalGeo& geo) const {
			geo.prim->PostIntersect(geo);
		}
		bool Scene::Occlude(const Ray& ray) const {
			return primmgr_->Occlude(ray);
		}
	}
}