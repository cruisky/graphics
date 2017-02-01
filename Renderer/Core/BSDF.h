#pragma once
#include "Math/Color.h"
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

	/// <summary>
	/// Bi-directional Scattering Distribution Function.
	/// </summary>
	class BSDF {
	public:
		BSDF(BSDFType t, const Color& c = Color::WHITE) : type_(t), color_(c){}
		virtual ~BSDF(){}

		virtual Color SampleDirect(const Vec3& wo, const LocalGeo& geom, const Sample& sample, Vec3 *wi, float *pdf, BSDFType types = BSDF_ALL, BSDFType *sampled_types = nullptr) const = 0;
		inline bool SubtypeOf(BSDFType t) const { return (type_ & t) == type_; }
		inline bool IsSpecular() const { return (BSDFType(BSDF_SPECULAR | BSDF_DIFFUSE | BSDF_GLOSSY) & type_) == BSDFType(BSDF_SPECULAR); }
		inline Color GetColor(const LocalGeo& geo) const {
			return color_;
		}
		virtual Color Eval(const Vec3& wo, const Vec3& wi, const LocalGeo& geom, BSDFType type = BSDF_ALL) const;
		virtual float Pdf(const Vec3& wo, const Vec3& wi, const LocalGeo& geom, BSDFType type = BSDF_ALL) const;

		// clumsy approximation of phong components
		virtual Color GetAmbient() const;
		virtual Color GetDiffuse() const;
		virtual Color GetSpecular() const;
		virtual float GetShininess() const;

	protected:
		virtual float Eval(const Vec3& localwo, const Vec3& localwi, BSDFType type = BSDF_ALL) const = 0;
		virtual float Pdf(const Vec3& localwo, const Vec3& localwi, BSDFType type = BSDF_ALL) const = 0;
		inline bool Valid(const Vec3& wo, const Vec3& wi, const Vec3& normal, BSDFType *t) const{
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

	/// <summary>
	/// Diffuse BSDF.
	/// </summary>
	class Diffuse : public BSDF {
	public:
		Diffuse(const Color& c = Color::WHITE) : BSDF(BSDFType(BSDF_REFLECTION | BSDF_DIFFUSE), c){}
		Color SampleDirect(const Vec3& wo, const LocalGeo& geom, const Sample& sample, Vec3 *wi, float *pdf, BSDFType types = BSDF_ALL, BSDFType *sampled_types = nullptr) const;
	protected:
		float Eval(const Vec3& wo, const Vec3& wi, BSDFType type = BSDF_ALL) const;
		float Pdf(const Vec3& wo, const Vec3& wi, BSDFType type = BSDF_ALL) const;

		virtual Color GetAmbient() const;
	};

	/// <summary>
	/// Mirror BSDF.
	/// </summary>
	class Mirror : public BSDF {
	public:
		Mirror(const Color& c = Color::WHITE) : BSDF(BSDFType(BSDF_REFLECTION | BSDF_SPECULAR), c){}
		Color SampleDirect(const Vec3& wo, const LocalGeo& geom, const Sample& sample, Vec3 *wi, float *pdf, BSDFType types = BSDF_ALL, BSDFType *sampled_types = nullptr) const;
		Color Eval(const Vec3& wo, const Vec3& wi, const LocalGeo& geom, BSDFType type = BSDF_ALL) const;
		float Pdf(const Vec3& wo, const Vec3& wi, const LocalGeo& geom, BSDFType type = BSDF_ALL) const;

		virtual Color GetDiffuse() const;
		virtual Color GetSpecular() const;
		virtual float GetShininess() const;
	protected:
		float Eval(const Vec3& wo, const Vec3& wi, BSDFType type = BSDF_ALL) const;
		float Pdf(const Vec3& wo, const Vec3& wi, BSDFType type = BSDF_ALL) const;
	};

	/// <summary>
	/// Dielectric BSDF.
	/// </summary>
	class Dielectric : public BSDF{
	public:
		Dielectric(const Color& c = Color::WHITE, float etat = 1.5f, float etai = 1.f) :
			BSDF(BSDFType(BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_SPECULAR), c),
			etat_(etat), etai_(etai), eta_(etai / etat), eta_inv_(etat / etai){}
		Color SampleDirect(const Vec3& wo, const LocalGeo& geom, const Sample& sample, Vec3 *wi, float *pdf, BSDFType types = BSDF_ALL, BSDFType *sampled_types = nullptr) const;
		Color Eval(const Vec3& wo, const Vec3& wi, const LocalGeo& geom, BSDFType type = BSDF_ALL) const;
		float Pdf(const Vec3& wo, const Vec3& wi, const LocalGeo& geom, BSDFType type = BSDF_ALL) const;

		virtual Color GetAmbient() const;
		virtual Color GetDiffuse() const;
		virtual Color GetSpecular() const;
		virtual float GetShininess() const;
	protected:
		float Eval(const Vec3& wo, const Vec3& wi, BSDFType type = BSDF_ALL) const;
		float Pdf(const Vec3& wo, const Vec3& wi, BSDFType type = BSDF_ALL) const;
		/// <summary>
		/// Computes cosine of out angle using Snell's law.
		/// </summary>
		/// <param name="cosi">Incident angle</param>
		/// <param name="eta">Index of refraction</param>
		float Refract(float cosi, float *eta = nullptr) const;
		/// <summary>
		/// Computes reflectance using Fresnel's equation.
		/// </summary>
		/// <param name="cosi">Incident angle</param>
		/// <param name="cost">Reflected angle</param>
		float Reflectance(float cosi, float cost) const;
	private:
		const float etai_, etat_;		// indices of refraction on each side
		const float eta_, eta_inv_;		// convenient constants
	};

	namespace LocalCoord {
		inline float CosTheta(const Vec3& vec) { return vec.z; }
		inline float CosTheta2(const Vec3& vec) { return vec.z * vec.z; }
		inline float AbsCosTheta(const Vec3& vec) { return Math::Abs(vec.z); }
		inline float SinTheta2(const Vec3& vec) { return Math::Max(0.f, 1.f - CosTheta2(vec)); }
		inline float SinTheta(const Vec3& vec) { return Math::Sqrt(SinTheta2(vec)); }
		inline float TanTheta(const Vec3& vec)
		{
			return Math::Sqrt(SinTheta2(vec)) / vec.z;
		}
		inline float TanTheta2(const Vec3& vec)
		{
			float cos2 = vec.z * vec.z;
			float sin2 = 1 - cos2;
			if (sin2 <= 0.f)
				return 0.f;
			return sin2 / cos2;
		}
		inline float CosPhi(const Vec3& vec)
		{
			float sintheta = SinTheta(vec);
			if (sintheta == 0.f) return 1.f;
			return Math::Clamp(vec.x / sintheta, -1.f, 1.f);
		}
		inline float SinPhi(const Vec3& vec)
		{
			float sintheta = SinTheta(vec);
			if (sintheta == 0.f)
				return 0.f;
			return Math::Clamp(vec.y / sintheta, -1.f, 1.f);
		}
		inline bool SameHemisphere(const Vec3& v1, const Vec3& v2) { return v1.z * v2.z > 0.f; }

	}
}
