#include "UtilStdAfx.h"
#include "Scene.h"
#include "PrimitiveManager.h"
#include "Primitive.h"
#include "Intersection.h"

namespace TX {
	Scene::Scene() {
		primmgr_ = std::unique_ptr<PrimitiveManager>(new PrimitiveManager(&prims_));
	}
	void Scene::AddPrimitive(std::shared_ptr<Primitive> prim){
		prims_.push_back(prim);
	}
	void Scene::AddLight(std::shared_ptr<Light> light){
		lights.push_back(light);
	}
	void Scene::Construct(){
		for (auto& obj : prims_) {
			obj->transform.Update();
		}
		for (auto& obj : lights) {
			obj->transform.Update();
		}
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
