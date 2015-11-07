#include "stdafx.h"
#include "DirectionalLight.h"
#include "Graphics/Intersection.h"

namespace TX {
	DirectionalLight::DirectionalLight(const Color& intensity, int sample_count) : Light(sample_count), intensity(intensity){}
	DirectionalLight::DirectionalLight(const Color& intensity, const Vec3& dir, int sample_count) :
		Light(sample_count), intensity(intensity), dir(dir) {}

	void DirectionalLight::SampleDirect(const Vec3& pos, const Sample *lightsamples, Ray *wi, Color *lightcolor, float *pdf) const {
		// the position of a directional light can be everywhere, so we only need to transform the direction
		*wi = Ray(pos, dir);
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
