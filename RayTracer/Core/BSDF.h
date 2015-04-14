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

			Color Eval(const Vector3& wi, const Vector3& wo, const LocalGeo& geo);

			inline Color GetAmbient(){ return ambient_; }
			inline Color GetDiffuse(){ return diffuse_; }
			inline Color GetSpecular(){ return specular_; }
			inline Color GetShininess(){ return shininess_; }

			virtual float GetReflection(float cos_incidence){ return reflection_; }
			virtual float GetRefractiveIndex(){ return NOT_REFRACTIVE; }
			virtual float GetRefractiveIndexInv(){ return NOT_REFRACTIVE; }
			virtual Color GetAttenuation(float ray_distance){ return Color(); }
		protected:
			Color ambient_, diffuse_, specular_;
			float shininess_, reflection_;
		};


		class Dielectric : protected BSDF{
		public:
			Dielectric(Color ambient, Color diffuse, Color specular, float shininess, float refractive_index, Color attenuation) :
				BSDF(ambient, diffuse, specular, shininess, NOT_REFLECTVIE), refr_index_(refractive_index){
				refr_index_inv_ = 1.f / refractive_index;
				refl_ = Math::Pow((refractive_index - 1.f) / (refractive_index + 1.f), 2.f);
				refl_c_ = 1.f - refl_;
				att_log_ = Math::Log(attenuation);
			}
			~Dielectric(){}

			float GetReflection(float cos_incidence) override { return (refl_ + refl_c_ * Math::Pow(1.f - cos_incidence, 5.f)); }
			float GetRefractiveIndex() override { return refr_index_; }
			float GetRefractiveIndexInv() override { return refr_index_inv_; }
			Color GetAttenuation(float ray_distance) override { return Math::Exp(att_log_ * -ray_distance); }
		private:
			float refr_index_, refr_index_inv_, refl_, refl_c_;
			Color att_log_;
		};
	}
}