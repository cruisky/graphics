#include "stdafx.h"
#include "PointLight.h"
#include "Graphics/Intersection.h"

namespace TX {
	PointLight::PointLight(const Color& intensity, float radius, const Vec3& position, int sample_count) :
		Light(sample_count), intensity(intensity){
		SetRadius(radius);
		if (position != Vec3::ZERO)
			transform.SetPosition(position);
	}

	void PointLight::Illuminate(const Vec3& pos, const Sample *lightsamples, Ray *wi, Color *lightcolor, float *pdf) const{
		wi->SetSegment(pos, transform.Position());
		*lightcolor = intensity * (1.f - wi->t_max * wi->t_max * radius_sqr_inv_);	// decrease brightness by dist^2
		*pdf = 1.f;
	}

	float PointLight::Pdf(const Vec3& pos, const Vec3& dir) const{
		return 0.f;
	}

	bool PointLight::IsDelta() const {
		return true;
	}
}