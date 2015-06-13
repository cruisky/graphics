#include "stdafx.h"
#include "DirectionalLight.h"
#include "Core/Intersection.h"
#include "Color.h"

namespace Cruisky {
	namespace RayTracer {
		DirectionalLight::DirectionalLight(const Color& intensity, int sample_count) : Light(sample_count), intensity(intensity){}
		DirectionalLight::DirectionalLight(const Color& intensity, const Vector3& dir, int sample_count) :
			Light(sample_count), intensity(intensity) {
			transform.LookAt(dir);
		}

		void DirectionalLight::Illuminate(const Vector3& pos, const Sample *lightsamples, Ray *out, Color *lightcolor, float *pdf) const {
			// the position of a directional light can be everywhere, so we only need to transform the direction
			*out = Ray(pos, Matrix4x4::TVector(transform.LocalToWorldMatrix(), Vector3(0.f, 0.f, 1.f)));
			*lightcolor = intensity;
			*pdf = 1.f;
		}

		float DirectionalLight::Pdf(const Vector3& pos, const Vector3& dir) const{
			return 0.f;
		}		
		
		bool DirectionalLight::IsDelta() const {
			return true;
		}
	}
}