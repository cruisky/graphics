#include "core/math.h"
#include "core/ray.h"
#include "rayhit.h"
#include "shape.h"

namespace Cruisky{
	namespace RayTracer
	{
		bool UnitSphere::Intersect(Ray& lcr, RayHit& hit) {
			float e2 = LengthSqr(lcr.origin);
			float d2 = LengthSqr(lcr.dir);
			float de = Dot(lcr.dir, lcr.origin);
			float delta_quarter = de * de - d2 * (e2 - 1);
			float t = -(de + Math::Sqrt(delta_quarter)) / d2;
			if (Math::InBounds(t, lcr.t_min, lcr.t_max) && delta_quarter > 0){
				lcr.t_max = t;
				hit.point = lcr.origin + t * lcr.dir;
				hit.normal = Normalized(hit.point);
				return true;
			}
			return false;
		}

		bool UnitSphere::Intersect(Ray& lcr) {
			float e2 = LengthSqr(lcr.origin);
			float d2 = LengthSqr(lcr.dir);
			float de = Dot(lcr.dir, lcr.origin);
			float delta_quarter = de * de - d2 * (e2 - 1);
			float t = -(de + Math::Sqrt(delta_quarter)) / d2;
			return delta_quarter > 0.f && Math::InBounds(t, lcr.t_min, lcr.t_max);
		}

		bool UnitPlane::Intersect(Ray& lcr, RayHit& hit) {
			float t = -lcr.origin.z / lcr.dir.z;
			Vector3 point = lcr.origin + t * lcr.dir;
			if (Math::InBounds(t, lcr.t_min, lcr.t_max) &&
				Math::InBounds(point.x, -0.5f, 0.5f) &&
				Math::InBounds(point.y, -0.5f, 0.5f)) {
				lcr.t_max = t;
				hit.point = point;
				hit.normal = point.Normalize();
				return true;
			}
			return false;
		}

		bool UnitPlane::Intersect(Ray& lcr) {
			float t = -lcr.origin.z / lcr.dir.z;
			return Math::InBounds(t, lcr.t_min, lcr.t_max);
		}
	}
}