#include "core/math.h"
#include "core/ray.h"
#include "rayhit.h"
#include "shape.h"

namespace Cruisky{

	bool UnitSphere::Intersect(const Ray& ray, RayHit& geo) {
		Vector3 e = ray.origin;
		Vector3 d = ray.dir;
		float e2 = LengthSqr(e);
		float d2 = LengthSqr(d);
		float de = Dot(d, e);
		float delta_quarter = de * de - d2 * (e2 - 1);
		float t = -(de + Math::Sqrt(delta_quarter)) / d2;
		if (geo.t > t && 
			delta_quarter > 0 && 
			Math::InBounds(t, ray.t_min, ray.t_max)){
			geo.t = t;
			geo.point = e + t * d;
			geo.normal = Normalized(geo.point);
			return true;
		}
		return false;
	}

	bool UnitSphere::Intersect(const Ray& ray) {
		Vector3 e = ray.origin;
		Vector3 d = ray.dir;
		float e2 = LengthSqr(e);
		float d2 = LengthSqr(d);
		float de = Dot(d, e);
		float delta_quarter = de * de - d2 * (e2 - 1);
		float t = -(de + Math::Sqrt(delta_quarter)) / d2;
		return delta_quarter > 0.f && Math::InBounds(t, ray.t_min, ray.t_max);
	}

	bool UnitPlane::Intersect(const Ray& ray, RayHit& hit) {
		Vector3 e = ray.origin;
		Vector3 d = ray.dir;
		float t = -e.z / d.z;
		Vector3 point = e + t * d;
		if (hit.t > t && 
			Math::InBounds(point.x, -0.5f, 0.5f) && 
			Math::InBounds(point.y, -0.5f, 0.5f) && 
			Math::InBounds(t, ray.t_min, ray.t_max)) {
			hit.t = t;
			hit.point = point;
			hit.normal = point.Normalize();
			return true;
		}
		return false;
	}

	bool UnitPlane::Intersect(const Ray& ray) {
		float t = -ray.origin.z / ray.dir.z;
		return t > 0.f && Math::InBounds(t, ray.t_min, ray.t_max);
	}
}