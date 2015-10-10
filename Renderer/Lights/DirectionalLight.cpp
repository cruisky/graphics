#include "stdafx.h"
#include "DirectionalLight.h"
#include "Graphics/Intersection.h"

namespace TX {
	DirectionalLight::DirectionalLight(const Color& intensity, int sample_count) : Light(sample_count), intensity(intensity){}
	DirectionalLight::DirectionalLight(const Color& intensity, const Vec3& dir, int sample_count) :
		Light(sample_count), intensity(intensity) {
		transform.LookAt(dir);
	}

	void DirectionalLight::Illuminate(const Vec3& pos, const Sample *lightsamples, Ray *wi, Color *lightcolor, float *pdf) const {
		// the position of a directional light can be everywhere, so we only need to transform the direction
		*wi = Ray(pos, Matrix4x4::TVec(transform.LocalToWorldMatrix(), Vec3(0.f, 0.f, 1.f)));
		*lightcolor = intensity;
		*pdf = 1.f;
	}

	float DirectionalLight::Pdf(const Vec3& pos, const Vec3& dir) const{
		return 0.f;
	}		
		
	bool DirectionalLight::IsDelta() const {
		return true;
	}
}