#include "stdafx.h"
#include "Vector.h"
#include "BSDF.h"
#include "Intersection.h"
#include "Primitive.h"
#include "Sample.h"

namespace Cruisky{
	namespace RayTracer
	{
		//
		// BSDF
		//
		Color BSDF::Eval(const Vector3& wo, const Vector3& wi, const LocalGeo& geom, BSDFType t) const {
			if (!Valid(wo, wi, geom.normal, &t))
				return 0.f;
			return GetColor(geom) * Eval(
				geom.WorldToLocal(wo),
				geom.WorldToLocal(wi),
				t);
		}

		float BSDF::Pdf(const Vector3& wo, const Vector3& wi, const LocalGeo& geom, BSDFType t) const {
			if (!Valid(wo, wi, geom.normal, &t))
				return 0.f;
			return Pdf(
				geom.WorldToLocal(wo),
				geom.WorldToLocal(wi), 
				t);
		}

		//
		// Diffuse
		//
		Color Diffuse::Scatter(const Vector3& wo, const LocalGeo& geom, const Sample& sample, Vector3 *wi, float *pdf, BSDFType types, BSDFType *sampled_types) const{
			Vector3 localwo = geom.WorldToLocal(wo).Normalize();
			Vector3 localwi = MonteCarlo::CosineHemisphere(sample.u, sample.v);
			if (localwo.z < 0.f)
				localwi.z *= -1.f;
			*wi = geom.LocalToWorld(localwi);
			if (!Valid(wo, *wi, geom.normal, &types)){
				*pdf = 0.f;
				return Color::BLACK;
			}
			*pdf = Pdf(localwo, localwi, types);
			if (sampled_types) *sampled_types = type_;
			return GetColor(geom) * Eval(localwo, localwi, types);
		}

		float Diffuse::Eval(const Vector3& localwo, const Vector3& localwi, BSDFType type) const{
			return 1.f;// Math::PI_INV;
		}

		float Diffuse::Pdf(const Vector3& localwo, const Vector3& localwi, BSDFType type) const{
			if (!LocalCoord::SameHemisphere(localwo, localwi))
				return 0.f;
			return LocalCoord::AbsCosTheta(localwi) * Math::PI_INV;
		}

		
		//
		// Mirror
		//
		Color Mirror::Eval(const Vector3& wo, const Vector3& wi, const LocalGeo& geom, BSDFType type) const {
			return Color::BLACK;
		}
		float Mirror::Pdf(const Vector3& wo, const Vector3& wi, const LocalGeo& geom, BSDFType type) const {
			return 0.f;
		}
		Color Mirror::Scatter(const Vector3& wo, const LocalGeo& geom, const Sample& sample, Vector3 *wi, float *pdf, BSDFType types, BSDFType *sampled_types) const {
			if (!SubtypeOf(types)){
				*pdf = 0.f;
				return Color::BLACK;
			}
			Vector3 localwo(geom.WorldToLocal(wo));
			Vector3 localwi(-localwo.x, -localwo.y, localwo.z);
			*wi = geom.LocalToWorld(localwi);
			*pdf = 1.f;
			if (sampled_types) *sampled_types = type_;
			return GetColor(geom) / LocalCoord::AbsCosTheta(localwi);
		}
		float Mirror::Eval(const Vector3& wo, const Vector3& wi, BSDFType type) const {
			return 0.f;
		}
		float Mirror::Pdf(const Vector3& wo, const Vector3& wi, BSDFType type) const {
			return 0.f;
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