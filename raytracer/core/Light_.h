#pragma once

#include "Transform.h"

namespace Cruisky {
	namespace RayTracer {
		
		class Light{
		public:
			Light(){}

			// Emits a light ray from point to the light source.
			inline void Emit(const RayHit& point, Ray& lightray, Color& lightcolor) const {
				RayHit localpoint = transform.ToLocal(point);
				GenerateRay(localpoint, lightray, lightcolor);
				lightray = transform.ToWorld(lightray);
			}

		protected:
			virtual void GenerateRay(RayHit& localpoint, Ray& localray, Color& lightcolor) const = 0;

		public:
			Transform transform;
		};
	}
}