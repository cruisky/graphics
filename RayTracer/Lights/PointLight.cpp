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
			Vector3 dir = transform.Position() - geo.point;
			*out = Ray(geo.point, dir);
			*lightcolor = intensity * (1.f - LengthSqr(dir) * radius_sqr_inv_);	// decrease brightness by dist^2
			lightcolor->Clamp();
		}
	}
}