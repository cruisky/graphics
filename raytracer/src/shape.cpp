#include "core/math.h"
#include "core/ray.h"
#include "rayhit.h"
#include "shape.h"

namespace Cruisky{
	namespace RayTracer
	{
		bool UnitSphere::Intersect(const Ray& ray, RayHit& hit) {
			float e2 = LengthSqr(ray.origin);
			float d2 = LengthSqr(ray.dir);
			float de = Dot(ray.dir, ray.origin);
			float delta_quarter = de * de - d2 * (e2 - 1);
			float t = -(de + Math::Sqrt(delta_quarter)) / d2;
			if (Math::InBounds(t, ray.t_min, hit.t) && delta_quarter > 0){
				hit.t = t;
				hit.point = ray.origin + t * ray.dir;
				hit.normal = Normalized(hit.point);
				return true;
			}
			return false;
		}

		bool UnitSphere::Intersect(const Ray& ray) {
			float e2 = LengthSqr(ray.origin);
			float d2 = LengthSqr(ray.dir);
			float de = Dot(ray.dir, ray.origin);
			float delta_quarter = de * de - d2 * (e2 - 1);
			float t = -(de + Math::Sqrt(delta_quarter)) / d2;
			return delta_quarter > 0.f && Math::InBounds(t, ray.t_min, ray.t_max);
		}

		bool UnitPlane::Intersect(const Ray& ray, RayHit& hit) {
			float t = -ray.origin.z / ray.dir.z;
			Vector3 point = ray.origin + t * ray.dir;
			if (Math::InBounds(t, ray.t_min, hit.t) &&
				Math::InBounds(point.x, -0.5f, 0.5f) &&
				Math::InBounds(point.y, -0.5f, 0.5f)) {
				hit.t = t;
				hit.point = point;
				hit.normal = point.Normalize();
				return true;
			}
			return false;
		}

		bool UnitPlane::Intersect(const Ray& ray) {
			float t = -ray.origin.z / ray.dir.z;
			return Math::InBounds(t, ray.t_min, ray.t_max);
		}
	}
}