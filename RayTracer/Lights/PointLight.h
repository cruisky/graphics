#pragma once

#include "fwddecl.h"
#include "Core/Light.h"
#include "Color.h"

namespace Cruisky{
	namespace RayTracer {
		class PointLight : public Light {
		public:
			PointLight(const Color& intensity, float radius = 10.f, const Vector3& position = Vector3::ZERO);

			void Emit(const LocalGeo& geo, Ray *out, Color *lightcolor) const;

		public:
			Color intensity;
			float radius;
		};
	}
}
