#pragma once
#include "fwddecl.h"

#include "Color.h"

namespace Cruisky{
	namespace RayTracer
	{
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

		inline int NumComponents(BSDFType t){

		}

		//
		// BSDF
		//
		class BSDF {
		public:
			BSDF(BSDFType t, const Color& c = Color::WHITE) : type_(t), color_(c){}
			virtual ~BSDF(){}
			
			virtual Color Scatter(const Vector3& wo, const LocalGeo& geom, const Sample& sample, Vector3 *wi, float *pdf, BSDFType types = BSDF_ALL, BSDFType *sampled_types = nullptr) const = 0;
			inline bool SubtypeOf(BSDFType t) const { return (type_ & t) == type_; }
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
				if (Dot(wo, normal) * Dot(wi, normal) < 0.f)
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

		/*class BSDF {
		public:
			static const float NOT_REFRACTIVE;
		public:
			virtual ~BSDF(){}

			virtual Color Eval(const Vector3& wi, const Vector3& wo, const LocalGeo& geo) const = 0;
		};

		class Diffuse : public BSDF{
		public:
			Diffuse(Color ambient, Color diffuse) : ambient(ambient), diffuse(diffuse){}
			~Diffuse(){}
			
			virtual Color Eval(const Vector3& wi, const Vector3& wo, const LocalGeo& geo) const;
		public:
			Color ambient, diffuse;
		};

		class Specular : public Diffuse {
		public:
			Specular(Color ambient, Color diffuse, Color specular, float shininess) :
				Diffuse(ambient, diffuse), specular(specular), shininess(shininess){}
			~Specular(){}

			virtual Color Eval(const Vector3& wi, const Vector3& wo, const LocalGeo& geo) const;
		public:
			Color specular;
			float shininess;
		};

		class Mirror : public Specular {
		public:
			Mirror(Color ambient, Color diffuse, Color specular, float shininess, float reflectivity) :
				Specular(ambient, diffuse, specular, shininess), reflectivity(reflectivity){}
			~Mirror(){}
		public:
			float reflectivity;
		};

		class Dielectric : public Specular {
		public:
			Dielectric(Color specular, float shininess, float refractive_index, Color attenuation) :
				Specular(Color::BLACK, Color::BLACK, specular, shininess), refr_index_(refractive_index){
				refr_index_inv_ = 1.f / refractive_index;
				refl_ = Math::Pow((refractive_index - 1.f) / (refractive_index + 1.f), 2.f);
				refl_c_ = 1.f - refl_;
				att_log_ = Math::Log(attenuation);
			}
			~Dielectric(){}

			float RefractiveIndex() const { return refr_index_; }
			float RefractiveIndexInv() const { return refr_index_inv_; }
			Color Attenuation(float ray_distance) const { return Math::Exp(att_log_ * -ray_distance); }
			float Reflectivity(float cos_incidence) const { return (refl_ + refl_c_ * Math::Pow(1.f - cos_incidence, 5.f)); }
		private:
			float refr_index_, refr_index_inv_, refl_, refl_c_;
			Color att_log_;
		};*/

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
}