#include "stdafx.h"
#include "Intersection.h"
#include "Primitive.h"

namespace TX
{
	namespace RayTracer
	{
		void LocalGeo::Emit(const Vector3& wo, Color *out) const {
			const AreaLight *arealight = prim->GetAreaLight();
			if (arealight)
				arealight->Emit(point, normal, wo, out);
		}
	}
}