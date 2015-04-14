#include "stdafx.h"
#include "Primitive.h"
#include "Ray.h"
#include "Shape.h"
#include "Intersection.h"

namespace Cruisky{

	namespace RayTracer
	{
		bool Primitive::Intersect(const Ray& ray, Intersection& intxn) const {
			// transform the ray into local space
			localray_ = ray;
			transform.ToLocal(localray_);

			if (!shape_->Intersect(localray_)) return false;
			intxn.prim = this;
			ray.t_max = localray_.t_max;
			return true;
		}

		void Primitive::PostIntersect(LocalGeo& geo) const {
			geo.bsdf = GetBSDF();
			shape_->PostIntersect(localray_, geo);
			// transform the hit point and normal to world space
			const Matrix4x4& local_world = transform.LocalToWorldMatrix();
			geo.point = Matrix4x4::TPoint(local_world, geo.point);
			geo.normal = Matrix4x4::TNormal(local_world, geo.normal);
		}

		bool Primitive::Occlude(const Ray& ray) const {
			Ray loc_ray(ray);	// don't cache shadow ray
			transform.ToLocal(loc_ray);
			return shape_->Occlude(loc_ray);
		}

		const BSDF* Primitive::GetBSDF() const {
			return bsdf_.get();
		}

	}

}