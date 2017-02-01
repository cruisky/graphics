#include "stdafx.h"
#include "Intersection.h"
#include "Primitive.h"

namespace TX
{
	void LocalGeo::Emit(const Vec3& wo, Color *out) const {
		const AreaLight *arealight = prim->GetAreaLight();
		if (arealight)
			arealight->Emit(point, normal, wo, out);
	}
}
