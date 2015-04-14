#include "stdafx.h"
#include "BSDF.h"

namespace Cruisky{
	namespace RayTracer
	{
		const float BSDF::NOT_REFRACTIVE = -1.f;
		const float BSDF::NOT_REFLECTVIE = -1.f;

		Color Eval(const Vector3& wi, const Vector3& wo, const LocalGeo& geo){
			return Color(1, 0, 0);
			//TODO
		}
	}
}