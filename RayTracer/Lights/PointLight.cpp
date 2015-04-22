#include "stdafx.h"
#include "PointLight.h"
#include "Core/Intersection.h"

namespace Cruisky {
	namespace RayTracer{
		PointLight::PointLight(const Color& intensity, float radius, const Vector3& position, int sample_count) :
			Light(sample_count), intensity(intensity){
			SetRadius(radius);
			if (position != Vector3::ZERO)
				transform.SetPosition(position);
		}

		void PointLight::Illuminate(const Vector3& pos, const Sample *lightsamples, Ray *out, Color *lightcolor, float *pdf) const{
			out->SetSegment(pos, transform.Position());
			*lightcolor = intensity * (1.f - out->t_max * out->t_max * radius_sqr_inv_);	// decrease brightness by dist^2
			lightcolor->Clamp();
			*pdf = 1.f;
		}

		void PointLight::Emit(const Vector3& dir, Color *out) const{
			*out = Color::BLACK;
		}

		float PointLight::Pdf(const Vector3& pos, const Vector3& dir) const{
			return 0.f;
		}

	}
}