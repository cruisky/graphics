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

		void PointLight::Emit(const LocalGeo& geo, Ray *out, Color *lightcolor) const {
			*out = Ray(geo.point, transform.Position() - geo.point);
			*lightcolor = intensity * Math::Clamp(1.f - LengthSqr(out->dir) * radius_sqr_inv_, 0.f, 1.f);	// decrease brightness by dist^2
		}
	}
}