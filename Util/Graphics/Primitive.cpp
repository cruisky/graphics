#include "UtilStdAfx.h"
#include "Primitive.h"
#include "Shape.h"
#include "Intersection.h"

namespace TX {
	//bool Primitive::Intersect(const Ray& ray, Intersection& intxn) const {
	//	// transform the ray into local space
	//	Ray localray = ray;
	//	transform.ToLocal(localray);
	//	if (!mesh_->Intersect(localray)) return false;
	//	intxn.prim = this;
	//	intxn.localray = localray;			// only store the localray if it intersects
	//	intxn.dist = intxn.localray.t_max;
	//	ray.t_max = intxn.localray.t_max;
	//	return true;
	//}

	void Primitive::PostIntersect(const Ray& ray, LocalGeo& geom) const {
		geom.point = ray.End();
		geom.bsdf = GetBSDF();
		mesh_->PostIntersect(geom);
		// transform the normal to world space
		geom.normal = Math::Normalize(Matrix4x4::TNormal(transform.WorldToLocalMatrix(), geom.normal));
	}

	//bool Primitive::Occlude(const Ray& ray) const {
	//	Ray loc_ray(ray);	// don't cache shadow ray
	//	transform.ToLocal(loc_ray);
	//	return mesh_->Occlude(loc_ray);
	//}

	const Mesh* Primitive::GetMesh() const {
		return mesh_.get();
	}

	const BSDF* Primitive::GetBSDF() const {
		return bsdf_.get();
	}

	const AreaLight* Primitive::GetAreaLight() const {
		return areaLight;
	}
}
