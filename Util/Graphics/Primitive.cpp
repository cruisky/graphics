#include "UtilStdAfx.h"
#include "Primitive.h"
#include "Shape.h"
#include "Intersection.h"

namespace TX {
	bool Primitive::Intersect(const Ray& ray, Intersection& intxn) const {
		// transform the ray into local space
		Ray localray = ray;
		transform.ToLocal(localray);
		if (!shape_->Intersect(localray)) return false;
		intxn.prim = this;
		intxn.localray = localray;			// only store the localray if it intersects
		intxn.dist = intxn.localray.t_max;
		ray.t_max = intxn.localray.t_max;
		return true;
	}

	void Primitive::PostIntersect(const Ray& ray, LocalGeo& geo) const {
		geo.point = ray.End();
		geo.bsdf = GetBSDF();
		shape_->PostIntersect(geo.localray, geo);
		// transform the normal to world space
		geo.normal = Matrix4x4::TNormal(transform.WorldToLocalMatrix(), geo.normal);
	}

	bool Primitive::Occlude(const Ray& ray) const {
		Ray loc_ray(ray);	// don't cache shadow ray
		transform.ToLocal(loc_ray);
		return shape_->Occlude(loc_ray);
	}

	const BSDF* Primitive::GetBSDF() const {
		return bsdf_.get();
	}

	const AreaLight* Primitive::GetAreaLight() const {
		return area_light_;
	}
}