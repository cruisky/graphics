#include "stdafx.h"
#include "BSDF.h"
#include "Intersection.h"
#include "Vector.h"

namespace Cruisky{
	namespace RayTracer
	{
		const float BSDF::NOT_REFRACTIVE = -1.f;
		const float BSDF::NOT_REFLECTVIE = -1.f;

		Color BSDF::Eval(const Vector3& wi, const Vector3& wo, const LocalGeo& geo) const {
			Color c = ambient_;
			Vector3 h = wi + wo; h.Normalize();
			c += diffuse_ * Math::Max(0.f, Dot(geo.normal, wi));
			c += specular_ * Math::Pow(Math::Max(0.f, Dot(h, geo.normal)), shininess_);
			return c.Clamp();
		}
	}
}