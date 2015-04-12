#include "stdafx.h"
#include "DirectionalLight.h"
#include "Color.h"

namespace Cruisky {
	namespace RayTracer {
		DirectionalLight::DirectionalLight(const Color& intensity, const Vector3& dir) : 
			intensity(intensity) {
			if (dir != Vector3::ZERO){
				transform.LookAt(dir);
			}
		}

		void DirectionalLight::Emit(const LocalGeo& geo, Ray& out, Color& lightcolor) const {
			// TODO
		}
	}
}