#include "core/ray.h"

namespace Cruisky{

	const float Ray::EPSILON = 1e-6f;

	inline Ray& Ray::operator = (const Ray& ot){
		origin = ot.origin;
		dir = ot.dir;
		t_min = ot.t_min;
		t_max = ot.t_max;
	}

}