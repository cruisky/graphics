#pragma once

#include "transform.h"

namespace Cruisky {
	namespace RayTracer {
		
		class Light{
		public:
			Light(){}
			Light(Vector3 position, Vector3 rotation = Vector3::ZERO, Vector3 scale = Vector3::ONE) :
				transform(position, rotation, scale){}

			virtual void Shade(Ray& ray) = 0;
		public:
			Transform transform;
		};
	}
}