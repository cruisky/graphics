#include "stdafx.h"
#include "BSDF.h"
#include "Intersection.h"
#include "Vector.h"

namespace Cruisky{
	namespace RayTracer
	{
		float BSDF::Eval(const Vector3& wo, const Vector3& wi, const LocalGeo& geo, BSDFType t = BSDF_ALL) const {
			if (Dot(wo, geo.normal) * Dot(wi, geo.normal) < 0.f)
				t = BSDFType(t & ~BSDF_REFLECTION);
			else
				t = BSDFType(t & ~BSDF_TRANSMISSION);
			if (!SubtypeOf(t))
				return 0.f;
			return Eval(wo, wi, t);
		}
		float BSDF::Pdf(const Vector3& wo, const Vector3& wi, const LocalGeo& geo, BSDFType t = BSDF_ALL) const {
			if (Dot(wo, geo.normal) * Dot(wi, geo.normal) < 0.f)
				t = BSDFType(t & ~BSDF_REFLECTION);
			else
				t = BSDFType(t & ~BSDF_TRANSMISSION);
			if (!SubtypeOf(t))
				return 0.f;
			return Pdf(wo, wi, t);
		}
		/*const float BSDF::NOT_REFRACTIVE = -1.f;

		Color Diffuse::Eval(const Vector3& wi, const Vector3& wo, const LocalGeo& geo) const {
			Color c = ambient;
			c += diffuse * Math::Max(0.f, Dot(geo.normal, wi));
			return c.Clamp();
		}

		Color Specular::Eval(const Vector3& wi, const Vector3& wo, const LocalGeo& geo) const {
			Color c = Diffuse::Eval(wi, wo, geo);
			Vector3 h = (wi + wo).Normalize();
			c += specular * Math::Pow(Math::Max(0.f, Dot(h, geo.normal)), shininess);
			return c.Clamp();
		}*/
	}
}