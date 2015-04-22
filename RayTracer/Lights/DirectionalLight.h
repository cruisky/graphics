#pragma once

#include "Core/Light.h"
#include "Color.h"

namespace Cruisky{
	namespace RayTracer{
		class DirectionalLight : public Light {
		public:
			DirectionalLight(const Color& intensity, int sample_count = 1);
			DirectionalLight(const Color& intensity, const Vector3& dir, int sample_count = 1);

			void Emit(const LocalGeo& geo, Ray *out, Color *lightcolor) const;

		private:
			Color intensity;
		};
	}
}