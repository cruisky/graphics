#pragma once

#include "Core/Light.h"
#include "Color.h"

namespace Cruisky{
	namespace RayTracer{
		class DirectionalLight : protected Light {
		public:
			DirectionalLight(const Color& intensity, const Vector3& dir);

			void Emit(const LocalGeo& geo, Ray& out, Color& lightcolor) const;

		private:
			Color intensity;
		};
	}
}