#include "stdafx.h"
#include "PointLight.h"

namespace Cruisky {
	namespace RayTracer{
		PointLight::PointLight(const Color& intensity, float radius, const Vector3& position, int sample_count) :
			Light(sample_count), intensity(intensity){
			SetRadius(radius);
			if (position != Vector3::ZERO)
				transform.SetPosition(position);
		}

		void PointLight::Emit(const LocalGeo& geo, Ray *out, Color *lightcolor) const {
			
			out->dir = (transform.Position() - geo.point).Normalize();
			out->origin = geo.point + Ray::EPSILON * out->dir;
			*lightcolor = intensity * Math::Clamp(1.f - LengthSqr(out->dir) * radius_sqr_inv_, 0.f, 1.f);	// decrease brightness by dist^2
		}
	}
}