#pragma once
#include "fwddecl.h"

#include "Color.h"

namespace Cruisky{
	namespace RayTracer
	{
		class BSDF {
		public:
			static const float NOT_REFRACTIVE;
			static const float NOT_REFLECTVIE;
		public:
			BSDF(Color ambient, Color diffuse, Color specular, float shininess, float reflection) :
				ambient_(ambient), diffuse_(diffuse), specular_(specular), shininess_(shininess), reflection_(reflection){}
			virtual ~BSDF(){}

			Color Eval(const Vector3& wi, const Vector3& wo, const LocalGeo& geo) const;

			inline Color Ambient() const { return ambient_; }
			inline Color Diffuse() const { return diffuse_; }
			inline Color Specular() const { return specular_; }
			inline Color Shininess() const { return shininess_; }

			virtual float Reflection(float cos_incidence) const { return reflection_; }
			virtual float RefractiveIndex() const { return NOT_REFRACTIVE; }
			virtual float RefractiveIndexInv() const { return NOT_REFRACTIVE; }
			virtual Color Attenuation(float ray_distance) const { return Color(); }
		protected:
			const Color ambient_, diffuse_, specular_;
			const float shininess_, reflection_;
		};


		class Dielectric : public BSDF{
		public:
			Dielectric(Color ambient, Color diffuse, Color specular, float shininess, float refractive_index, Color attenuation) :
				BSDF(ambient, diffuse, specular, shininess, NOT_REFLECTVIE), refr_index_(refractive_index){
				refr_index_inv_ = 1.f / refractive_index;
				refl_ = Math::Pow((refractive_index - 1.f) / (refractive_index + 1.f), 2.f);
				refl_c_ = 1.f - refl_;
				att_log_ = Math::Log(attenuation);
			}
			~Dielectric(){}

			float Reflection(float cos_incidence) const override { return (refl_ + refl_c_ * Math::Pow(1.f - cos_incidence, 5.f)); }
			float RefractiveIndex() const override { return refr_index_; }
			float RefractiveIndexInv() const override { return refr_index_inv_; }
			Color Attenuation(float ray_distance) const override { return Math::Exp(att_log_ * -ray_distance); }
		private:
			float refr_index_, refr_index_inv_, refl_, refl_c_;
			Color att_log_;
		};
	}
}