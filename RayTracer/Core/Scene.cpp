#include "stdafx.h"
#include "Scene.h"
#include "PrimitiveManager.h"
#include "Primitive.h"
#include "Intersection.h"

namespace TX {
	namespace RayTracer {
		Scene::Scene(shared_ptr<Camera> camera) : camera(camera) {
			primmgr_ = unique_ptr<PrimitiveManager>(new PrimitiveManager(&prims_));
		}
		void Scene::AddPrimitive(shared_ptr<Primitive> prim){
			prims_.push_back(prim);
		}
		void Scene::AddLight(shared_ptr<Light> light){
			lights.push_back(light);
		}
		void Scene::Construct(){
			primmgr_->Construct();
		}
		bool Scene::Intersect(const Ray& ray, Intersection& intxn) const {
			return primmgr_->Intersect(ray, intxn);
		}
		void Scene::PostIntersect(const Ray& ray, LocalGeo& geo) const {
			geo.prim->PostIntersect(ray, geo);
		}
		bool Scene::Occlude(const Ray& ray) const {
			return primmgr_->Occlude(ray);
		}
	}
}