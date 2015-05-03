#include "stdafx.h"
#include "Vector.h"
#include "BSDF.h"
#include "Intersection.h"
#include "Primitive.h"
#include "Sample.h"

namespace Cruisky{
	namespace RayTracer
	{
		float BSDF::Eval(const Vector3& wo, const Vector3& wi, const LocalGeo& geom, BSDFType t) const {
			if (Dot(wo, geom.normal) * Dot(wi, geom.normal) < 0.f)
				t = BSDFType(t & ~BSDF_REFLECTION);
			else
				t = BSDFType(t & ~BSDF_TRANSMISSION);
			if (!SubtypeOf(t))
				return 0.f;
			return Eval(
				geom.WorldToLocal(wo),
				geom.WorldToLocal(wi),
				t);
		}

		float BSDF::Pdf(const Vector3& wo, const Vector3& wi, const LocalGeo& geom, BSDFType t) const {
			if (Dot(wo, geom.normal) * Dot(wi, geom.normal) < 0.f)
				t = BSDFType(t & ~BSDF_REFLECTION);
			else
				t = BSDFType(t & ~BSDF_TRANSMISSION);
			if (!SubtypeOf(t))
				return 0.f;
			return Pdf(
				geom.WorldToLocal(wo),
				geom.WorldToLocal(wi), 
				t);
		}

		float Diffuse::Eval(const Vector3& localwo, const Vector3& localwi, BSDFType type) const{
			return Math::PI_INV;
		}

		float Diffuse::Pdf(const Vector3& localwo, const Vector3& localwi, BSDFType type) const{
			if (!LocalCoord::SameHemisphere(localwo, localwi))
				return 0.f;
			return LocalCoord::AbsCosTheta(localwi) * Math::PI_INV;
		}

		Color Diffuse::Scatter(const Vector3& wo, const LocalGeo& geo, const Sample& sample, Vector3 *wi, float *pdf, BSDFType types, BSDFType *sampled_types) const{
			Vector3 localwo = geo.WorldToLocal(wo);
			Vector3 localwi = MonteCarlo::CosineHemisphere(sample.u, sample.v);
			if (localwo.z < 0.f)
				localwi.z *= -1.f;
			*wi = geo.LocalToWorld(localwi);
			if (Dot(wo, geo.normal) * Dot(*wi, geo.normal) < 0.f)
				types = BSDFType(types & ~BSDF_REFLECTION);
			else
				types = BSDFType(types & ~BSDF_TRANSMISSION);
			if (!SubtypeOf(types)){
				*pdf = 0.f;
				return Color::BLACK;
			}
			*pdf = Pdf(localwo, localwi, types);
			if (sampled_types) *sampled_types = type;
			return GetColor(geo) * Eval(localwo, localwi, types);
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