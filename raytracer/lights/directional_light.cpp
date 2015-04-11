#include "directional_light.h"

namespace Cruisky {
	namespace RayTracer {
		DirectionalLight::DirectionalLight(const Color& intensity, const Vector3& dir) : 
			intensity(intensity) {
			if (dir != Vector3::ZERO){
				transform.LookAt(dir);
			}
		}

		void DirectionalLight::GenerateRay(RayHit& localpoint, Ray& localray, Color& lightcolor) const {
			// TODO
		}
	}
}