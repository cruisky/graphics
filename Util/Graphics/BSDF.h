#pragma once
#include "Graphics/Color.h"
#include "Math/Vector.h"

namespace TX{
	enum BSDFType {
		BSDF_REFLECTION		= 1<<0,
		BSDF_TRANSMISSION	= 1<<1,
		BSDF_DIFFUSE		= 1<<2,
		BSDF_GLOSSY			= 1<<3,
		BSDF_SPECULAR		= 1<<4,
		BSDF_ALL_TYPES			= BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR,
		BSDF_ALL_REFLECTION		= BSDF_REFLECTION | BSDF_ALL_TYPES,
		BSDF_ALL_TRANSMISSION	= BSDF_TRANSMISSION | BSDF_ALL_TYPES,
		BSDF_ALL				= BSDF_ALL_REFLECTION | BSDF_ALL_TRANSMISSION
	};

	//
	// BSDF
	//
	class BSDF {
	public:
		BSDF(BSDFType t, const Color& c = Color::WHITE) : type_(t), color_(c){}
		virtual ~BSDF(){}
			
		virtual Color Scatter(const Vector3& wo, const LocalGeo& geom, const Sample& sample, Vector3 *wi, float *pdf, BSDFType types = BSDF_ALL, BSDFType *sampled_types = nullptr) const = 0;
		inline bool SubtypeOf(BSDFType t) const { return (type_ & t) == type_; }
		inline bool IsSpecular() const { return (BSDFType(BSDF_SPECULAR | BSDF_DIFFUSE | BSDF_GLOSSY) & type_) == BSDFType(BSDF_SPECULAR); }
		inline Color GetColor(const LocalGeo& geo) const {
			return color_;
		}

		// Wrappers
		virtual Color Eval(const Vector3& wo, const Vector3& wi, const LocalGeo& geom, BSDFType type = BSDF_ALL) const;
		virtual float Pdf(const Vector3& wo, const Vector3& wi, const LocalGeo& geom, BSDFType type = BSDF_ALL) const;

	protected:
		virtual float Eval(const Vector3& localwo, const Vector3& localwi, BSDFType type = BSDF_ALL) const = 0;
		virtual float Pdf(const Vector3& localwo, const Vector3& localwi, BSDFType type = BSDF_ALL) const = 0;
		inline bool Valid(const Vector3& wo, const Vector3& wi, const Vector3& normal, BSDFType *t) const{
			if (Math::Dot(wo, normal) * Math::Dot(wi, normal) < 0.f)
				*t = BSDFType(*t & ~BSDF_REFLECTION);
			else
				*t = BSDFType(*t & ~BSDF_TRANSMISSION);
			return SubtypeOf(*t);
		}

	protected:
		const BSDFType type_;
		const Color color_;
	};

	//
	// Diffuse
	//
	class Diffuse : public BSDF {
	public:
		Diffuse(const Color& c = Color::WHITE) : BSDF(BSDFType(BSDF_REFLECTION | BSDF_DIFFUSE), c){}
		Color Scatter(const Vector3& wo, const LocalGeo& geom, const Sample& sample, Vector3 *wi, float *pdf, BSDFType types = BSDF_ALL, BSDFType *sampled_types = nullptr) const;
	protected:
		float Eval(const Vector3& wo, const Vector3& wi, BSDFType type = BSDF_ALL) const;
		float Pdf(const Vector3& wo, const Vector3& wi, BSDFType type = BSDF_ALL) const;
	};

	//
	// Mirror
	//
	class Mirror : public BSDF {
	public:
		Mirror(const Color& c = Color::WHITE) : BSDF(BSDFType(BSDF_REFLECTION | BSDF_SPECULAR), c){}
		Color Scatter(const Vector3& wo, const LocalGeo& geom, const Sample& sample, Vector3 *wi, float *pdf, BSDFType types = BSDF_ALL, BSDFType *sampled_types = nullptr) const;
		Color Eval(const Vector3& wo, const Vector3& wi, const LocalGeo& geom, BSDFType type = BSDF_ALL) const;
		float Pdf(const Vector3& wo, const Vector3& wi, const LocalGeo& geom, BSDFType type = BSDF_ALL) const;
	protected:
		float Eval(const Vector3& wo, const Vector3& wi, BSDFType type = BSDF_ALL) const;
		float Pdf(const Vector3& wo, const Vector3& wi, BSDFType type = BSDF_ALL) const;
	};

	//
	// Dielectric
	//
	class Dielectric : public BSDF{
	public:
		Dielectric(const Color& c = Color::WHITE, float etat = 1.5f, float etai = 1.f) :
			BSDF(BSDFType(BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_SPECULAR), c),
			etat_(etat), etai_(etai), eta_(etai / etat), eta_inv_(etat / etai){}
		Color Scatter(const Vector3& wo, const LocalGeo& geom, const Sample& sample, Vector3 *wi, float *pdf, BSDFType types = BSDF_ALL, BSDFType *sampled_types = nullptr) const;
		Color Eval(const Vector3& wo, const Vector3& wi, const LocalGeo& geom, BSDFType type = BSDF_ALL) const;
		float Pdf(const Vector3& wo, const Vector3& wi, const LocalGeo& geom, BSDFType type = BSDF_ALL) const;
	protected:
		float Eval(const Vector3& wo, const Vector3& wi, BSDFType type = BSDF_ALL) const;
		float Pdf(const Vector3& wo, const Vector3& wi, BSDFType type = BSDF_ALL) const;
		float Refract(float cosi, float *eta = nullptr) const;		// computes cosine of out angle using Snell's law
		float Reflectance(float cosi, float cost) const;	// Computes reflectance using Fresnel equation
	private:
		const float etai_, etat_;		// indices of refraction on each side
		const float eta_, eta_inv_;		// convenient constants
	};

	namespace LocalCoord {
		inline float CosTheta(const Vector3& vec) { return vec.z; }
		inline float CosTheta2(const Vector3& vec) { return vec.z * vec.z; }
		inline float AbsCosTheta(const Vector3& vec) { return Math::Abs(vec.z); }
		inline float SinTheta2(const Vector3& vec) { return Math::Max(0.f, 1.f - CosTheta2(vec)); }
		inline float SinTheta(const Vector3& vec) { return Math::Sqrt(SinTheta2(vec)); }
		inline float TanTheta(const Vector3& vec)
		{
			return Math::Sqrt(SinTheta2(vec)) / vec.z;
		}
		inline float TanTheta2(const Vector3& vec)
		{
			float cos2 = vec.z * vec.z;
			float sin2 = 1 - cos2;
			if (sin2 <= 0.f)
				return 0.f;
			return sin2 / cos2;
		}
		inline float CosPhi(const Vector3& vec)
		{
			float sintheta = SinTheta(vec);
			if (sintheta == 0.f) return 1.f;
			return Math::Clamp(vec.x / sintheta, -1.f, 1.f);
		}
		inline float SinPhi(const Vector3& vec)
		{
			float sintheta = SinTheta(vec);
			if (sintheta == 0.f)
				return 0.f;
			return Math::Clamp(vec.y / sintheta, -1.f, 1.f);
		}
		inline bool SameHemisphere(const Vector3& v1, const Vector3& v2) { return v1.z * v2.z > 0.f; }

	}
}