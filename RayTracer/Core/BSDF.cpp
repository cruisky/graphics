#include "stdafx.h"
#include "BSDF.h"

namespace Cruisky{
	namespace RayTracer
	{
		const float BSDF::NOT_REFRACTIVE = -1.f;
		const float BSDF::NOT_REFLECTVIE = -1.f;

		Color BSDF::Eval(const Vector3& wi, const Vector3& wo, const LocalGeo& geo) const {
			return Color(0, 0.5f, 1.f);
			//TODO
		}
	}
}