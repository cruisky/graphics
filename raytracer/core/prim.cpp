#include "stdafx.h"
#include "prim.h"
#include "ray.h"
#include "shape.h"
#include "rayhit.h"
#include "intersection.h"
#include "material.h"

namespace Cruisky{

	namespace RayTracer
	{
		bool GeometricPrimitive::Intersect(Ray& ray, Intersection& intxn) const {
			// transform the ray into local space
			Ray localRay = transform.ToLocal(ray);
			RayHit hit;

			if (shape->Intersect(localRay, hit)) return false;
			intxn.hit = transform.ToWorld(hit);
			intxn.prim = this;
			ray.t_max = localRay.t_max;
			return true;
		}

		bool GeometricPrimitive::Occlude(const Ray& ray) const {
			return shape->Occlude(transform.ToLocal(ray));
		}

		/*void GeometricPrimitive::GetBRDF(RayHit& hit, BRDF* out) const {
			material->GetBRDF(hit, out);
		}*/

	}

}