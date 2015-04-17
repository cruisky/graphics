#pragma once
#include "fwddecl.h"

#include "Color.h"

namespace Cruisky{
	namespace RayTracer
	{
		class BSDF {
		public:
			static const float NOT_REFRACTIVE;
		public:
			virtual ~BSDF(){}

			virtual Color Eval(const Vector3& wi, const Vector3& wo, const LocalGeo& geo) const = 0;

			virtual float RefractiveIndex() const { return NOT_REFRACTIVE; }
			virtual float RefractiveIndexInv() const { return NOT_REFRACTIVE; }
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
			Mirror(Color ambient, Color diffuse, Color specular, float shininess, float reflection) :
				Specular(ambient, diffuse, specular, shininess), reflection(reflection){}
			~Mirror(){}
			
			//virtual Color Eval(const Vector3& wi, const Vector3& wo, const LocalGeo& geo) const;
		public:
			float reflection;
		};

		class Dielectric : public Specular {
		public:
			Dielectric(Color ambient, Color diffuse, Color specular, float shininess, float refractive_index, Color attenuation) :
				Specular(ambient, diffuse, specular, shininess), refr_index_(refractive_index){
				refr_index_inv_ = 1.f / refractive_index;
				refl_ = Math::Pow((refractive_index - 1.f) / (refractive_index + 1.f), 2.f);
				refl_c_ = 1.f - refl_;
				att_log_ = Math::Log(attenuation);
			}
			~Dielectric(){}

			float RefractiveIndex() const override { return refr_index_; }
			float RefractiveIndexInv() const override { return refr_index_inv_; }
			
			Color Attenuation(float ray_distance) const { return Math::Exp(att_log_ * -ray_distance); }
			float Reflection(float cos_incidence) const { return (refl_ + refl_c_ * Math::Pow(1.f - cos_incidence, 5.f)); }
		private:
			float refr_index_, refr_index_inv_, refl_, refl_c_;
			Color att_log_;
		};
	}
}