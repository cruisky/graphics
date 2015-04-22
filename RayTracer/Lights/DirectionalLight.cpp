#include "stdafx.h"
#include "DirectionalLight.h"
#include "Color.h"

namespace Cruisky {
	namespace RayTracer {
		DirectionalLight::DirectionalLight(const Color& intensity, int sample_count) : Light(sample_count), intensity(intensity){}
		DirectionalLight::DirectionalLight(const Color& intensity, const Vector3& dir, int sample_count) :
			Light(sample_count), intensity(intensity) {
			transform.LookAt(dir);
		}

		void DirectionalLight::Emit(const LocalGeo& geo, Ray *out, Color *lightcolor) const {
			// the position of a directional light can be everywhere, so we only need to transform the direction
			out->dir = Matrix4x4::TVector(transform.LocalToWorldMatrix(), Vector3(0.f, 0.f, 1.f)).Normalize();
			out->origin = geo.point + Ray::EPSILON * out->dir;
			*lightcolor = intensity;
		}
	}
}