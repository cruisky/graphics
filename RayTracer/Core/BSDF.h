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

		class BSDF {
		public:
			BSDF(BSDFType t, const Color& c) : type(t), color(c){}
			virtual ~BSDF(){}

			// Wrappers

			virtual float Eval(const Vector3& wo, const Vector3& wi, const LocalGeo& geo, BSDFType type = BSDF_ALL) const;
			virtual float Pdf(const Vector3& wo, const Vector3& wi, const LocalGeo& geo, BSDFType type = BSDF_ALL) const;
			
			/*virtual Color Scatter(const Vector3& wo, const LocalGeo& geo, const Sample *samples, Vector3 *wi, float *pdf, BSDFType type = BSDF_ALL) const = 0;*/

			inline bool SubtypeOf(BSDFType t) const { return (type & t) == type; }
			inline Color GetColor(const LocalGeo& geo) const {
				return color;
			}

		protected:
			virtual float Eval(const Vector3& wo, const Vector3& wi, BSDFType type = BSDF_ALL) const;
			virtual float Pdf(const Vector3& wo, const Vector3& wi, BSDFType type = BSDF_ALL) const;
		protected:
			const BSDFType type;
			const Color color;
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
	}
}