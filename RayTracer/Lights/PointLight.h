#pragma once

#include "fwddecl.h"
#include "Core/Light.h"
#include "Color.h"

namespace Cruisky{
	namespace RayTracer {
		class PointLight : public Light {
		public:
			PointLight(const Color& intensity, float radius = 10.f, const Vector3& position = Vector3::ZERO, int sample_count = 1);

			void Emit(const LocalGeo& geo, Ray *out, Color *lightcolor) const;

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
}
