#include "stdafx.h"
#include "PointLight.h"

namespace Cruisky {
	namespace RayTracer{
		PointLight::PointLight(const Color& intensity, float radius, const Vector3& position) :
			intensity(intensity){
			SetRadius(radius);
			if (position != Vector3::ZERO)
				transform.SetPosition(position);
		}

		void PointLight::Emit(const LocalGeo& geo, Ray *out, Color *lightcolor) const {
			out->origin = geo.point;
			out->dir = transform.Position() - out->origin;
			*lightcolor = intensity * Math::Clamp(1.f - LengthSqr(out->dir) * radius_sqr_inv_, 0.f, 1.f);	// decrease brightness by dist^2
			out->dir.Normalize();
		}
	}
}