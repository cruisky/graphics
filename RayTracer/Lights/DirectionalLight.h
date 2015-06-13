#pragma once

#include "Core/Light.h"
#include "Color.h"

namespace Cruisky{
	namespace RayTracer{
		class DirectionalLight : public Light {
		public:
			DirectionalLight(const Color& intensity, int sample_count = 1);
			DirectionalLight(const Color& intensity, const Vector3& dir, int sample_count = 1);

			void Illuminate(const Vector3& pos, const Sample *lightsamples, Ray *wi, Color *lightcolor, float *pdf) const;
			float Pdf(const Vector3& pos, const Vector3& dir) const;
			bool IsDelta() const; 
		public:
			Color intensity;
		};
	}
}