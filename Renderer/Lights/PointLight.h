#pragma once
#include "Graphics/Light.h"

namespace TX{
	class PointLight : public Light {
	public:
		PointLight(const Color& intensity, float radius = 10.f, const Vector3& position = Vector3::ZERO, int sample_count = 1);

		void Illuminate(const Vector3& pos, const Sample *lightsamples, Ray *wi, Color *lightcolor, float *pdf) const;
		float Pdf(const Vector3& pos, const Vector3& dir) const;
		bool IsDelta() const;

		inline float Radius() const { return radius_; }
		inline PointLight& SetRadius(float radius){ 
			radius_ = radius; 
			radius_sqr_inv_ = 1.f / (radius * radius); 
			return *this; 
		}

	public:
		Color intensity;

	private:
		float radius_;
		float radius_sqr_inv_;
	};
}
