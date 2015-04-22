#include "stdafx.h"
#include "Scene.h"
#include "Light.h"

namespace Cruisky{
	namespace RayTracer{
		void VisibilityTester::SetSegment(const Vector3& origin, const Vector3& dest){
			ray_.origin = origin;
			ray_.dir = dest - origin;
			ray_.t_min = Ray::EPSILON;
			ray_.t_max = Length(ray_.dir);
			ray_.dir /= ray_.t_max;
		}
		bool VisibilityTester::Unocclude(const Scene *scene) const {
			return !scene->Occlude(ray_);
		}
	}
}