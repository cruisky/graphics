#include "stdafx.h"
#include "PointLight.h"

namespace Cruisky {
	namespace RayTracer{
		PointLight::PointLight(const Color& intensity, float radius, const Vector3& position) :
			intensity(intensity), radius(radius){
			if (position != Vector3::ZERO)
				transform.SetPosition(position);
		}

		void PointLight::Emit(const LocalGeo& geo, Ray *out, Color *lightcolor) const {
			out->origin = geo.point;
			out->dir = transform.Position() - out->origin;
			float brightness = Math::Clamp(1.f - LengthSqr(out->dir) / radius, 0.f, 1.f);
			*lightcolor = intensity * brightness;
			out->dir.Normalize();
		}
	}
}