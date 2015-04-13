#pragma once
#include "fwddecl.h"

#include "Transform.h"
#include "Intersection.h"

namespace Cruisky {
	namespace RayTracer {
		
		class Light{
		public:
			virtual ~Light(){};

			// Emits a light ray from point to the light source.
			virtual void Emit(const LocalGeo& geo, Ray& out, Color& lightcolor) const = 0;
		public:
			Transform transform;
		};
	}
}