#include "primitive.h"
#include "core/ray.h"
#include "shape.h"
#include "rayhit.h"
#include "intersection.h"
#include "material.h"

namespace Cruisky{

	namespace RayTracer
	{
		bool GeometricPrimitive::Intersect(Ray& ray, Intersection& intxn) {
			// transform the ray into local space
			Ray localRay = transform.ToLocal(ray);
			RayHit hit;

			if (shape->Intersect(localRay, hit)) return false;
			intxn.hit = transform.ToWorld(hit);
			intxn.prim = this;
			ray.t_max = localRay.t_max;
			return true;
		}

		bool GeometricPrimitive::Intersect(const Ray& ray) {
			return shape->Intersect(transform.ToLocal(ray));
		}

		/*void GeometricPrimitive::GetBRDF(RayHit& hit, BRDF* out) const {
			material->GetBRDF(hit, out);
		}*/
	}

}