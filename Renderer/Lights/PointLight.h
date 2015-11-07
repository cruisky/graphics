#pragma once
#include "Graphics/Light.h"

namespace TX{
	class PointLight : public Light {
	public:
		PointLight(const Color& intensity, float radius = 10.f, const Vec3& position = Vec3::ZERO, int sample_count = 1);

		void SampleDirect(const Vec3& pos, const Sample *lightsamples, Ray *wi, Color *lightcolor, float *pdf) const;
		float Pdf(const Vec3& pos, const Vec3& dir) const;
		bool IsDelta() const;

		inline float Radius() const { return radius; }
		inline PointLight& SetRadius(float radius){
			this->radius = radius;
			radiusSqRcp = 1.f / (radius * radius);
			return *this;
		}

	public:
		Color intensity;

	private:
		Vec3 pos;
		float radius;
		float radiusSqRcp;
	};
}
