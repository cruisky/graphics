#include "stdafx.h"
#include "Math/Vector.h"
#include "BSDF.h"
#include "Intersection.h"
#include "Primitive.h"
#include "Sample.h"

namespace TX{
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
			Vector3 localwi = Sampling::CosineHemisphere(sample.u, sample.v);
			if (localwo.z < 0.f)
				localwi.z *= -1.f;
			*wi = geom.LocalToWorld(localwi).Normalize();
			if (!Valid(wo, *wi, geom.normal, &types)){
				*pdf = 0.f;
				return Color::BLACK;
			}
			*pdf = Pdf(localwo, localwi, types);
			if (sampled_types) *sampled_types = type_;
			return GetColor(geom) * Eval(localwo, localwi, types);
		}

		float Diffuse::Eval(const Vector3& localwo, const Vector3& localwi, BSDFType type) const{
			return Math::PI_RCP;
		}

		float Diffuse::Pdf(const Vector3& localwo, const Vector3& localwi, BSDFType type) const{
			if (!LocalCoord::SameHemisphere(localwo, localwi))
				return 0.f;
			return LocalCoord::AbsCosTheta(localwi) * Math::PI_RCP;
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

		
		//
		// Dielectric
		//
		Color Dielectric::Scatter(const Vector3& wo, const LocalGeo& geom, const Sample& sample, Vector3 *wi, float *pdf, BSDFType types, BSDFType *sampled_types) const{
			bool reflection = (types & (BSDF_REFLECTION | BSDF_SPECULAR)) == (BSDF_REFLECTION | BSDF_SPECULAR);
			bool transmission = (types & (BSDF_TRANSMISSION | BSDF_SPECULAR)) == (BSDF_TRANSMISSION | BSDF_SPECULAR);
			if (!reflection && !transmission){
				*pdf = 0.f;
				return Color::BLACK;
			}
			bool both = reflection == transmission;
			Vector3 localwo(geom.WorldToLocal(wo)), localwi;
			// angle of refraction
			float eta;
			float cosi = LocalCoord::CosTheta(localwo), cost = Refract(cosi, &eta);
			// reflectance
			float refl = Reflectance(cosi, cost);
			float prob = 0.5f * refl + 0.25f;
			if (sample.w <= prob && both || reflection && !both){			// sample reflection
				localwi = Vector3(-localwo.x, -localwo.y, localwo.z);
				*wi = geom.LocalToWorld(localwi);
				*pdf = both ? prob : 1.f;
				if (sampled_types) *sampled_types = BSDFType(BSDF_REFLECTION | BSDF_SPECULAR);
				return GetColor(geom).Luminance() * refl / LocalCoord::AbsCosTheta(localwi);
			}
			else if(sample.w > prob && both || transmission && !both){		// sample refraction
				if (eta == eta_)	// entering
					cost = -cost;
				localwi = Vector3(eta * -localwo.x, eta * -localwo.y, cost);
				*wi = geom.LocalToWorld(localwi);
				*pdf = both ? 1.f - prob : prob;
				if (sampled_types) *sampled_types = BSDFType(BSDF_TRANSMISSION | BSDF_SPECULAR);
				return GetColor(geom) * ((1.f - refl) / LocalCoord::AbsCosTheta(localwi));
			}
			return Color::BLACK;
		}
		Color Dielectric::Eval(const Vector3& wo, const Vector3& wi, const LocalGeo& geom, BSDFType type) const {
			return Color::BLACK;
		}
		float Dielectric::Pdf(const Vector3& wo, const Vector3& wi, const LocalGeo& geom, BSDFType type) const {
			return 0.f;
		}
		float Dielectric::Eval(const Vector3& wo, const Vector3& wi, BSDFType type) const{
			return 0.f;
		}
		float Dielectric::Pdf(const Vector3& wo, const Vector3& wi, BSDFType type) const{
			return 0.f;
		}
		float Dielectric::Refract(float cosi, float *eta) const{
			float e = (cosi > 0.f) ? eta_ : eta_inv_;	// determine whether the ray is entering the surface
			if (eta) *eta = e;
			return Math::Sqrt(Math::Max(0.f, 1.f - e * e * (1.f - cosi * cosi)));
		}
		float Dielectric::Reflectance(float cosi, float cost) const {
			if (cost == 0.f) return 1.f;	// total internal reflection
			cosi = Math::Abs(cosi);
			float etci = etat_ * cosi, etct = etat_ * cost,
				eici = etai_ * cosi, eict = etai_ * cost;
			float para = (etci - eict) / (etci + eict);
			float perp = (eici - etct) / (eici + etct);
			return 0.5f * (para * para + perp * perp);
		}
	}
}