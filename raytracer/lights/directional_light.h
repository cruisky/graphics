#pragma once

#include "core/light.h"
#include "color.h"

namespace Cruisky{
	namespace RayTracer{
		class DirectionalLight : protected Light {
		public:
			DirectionalLight(const Color& intensity, const Vector3& dir);

		protected:
			void GenerateRay(RayHit& localpoint, Ray& localray, Color& lightcolor) const override;

		private:
			Color intensity;
		};
	}
}