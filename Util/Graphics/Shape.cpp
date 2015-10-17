#include "UtilStdAfx.h"
#include "Intersection.h"
#include "Shape.h"
#include "Math/Sample.h"

namespace TX{
	float Shape::Pdf(const Vec3& localpoint) const { return 1.f / Area(); }

	float Shape::Pdf(const Ray& localwi) const {
		LocalGeo geom;
		// Test intersection between the ray and this shape which should be an area light source
		if (!Intersect(localwi)) return 0.f;
		PostIntersect(localwi, geom);
		float pdf = localwi.t_max * localwi.t_max / (Math::AbsDot(geom.normal, -localwi.dir) * Area());
		return Math::IsINF(pdf) ? 0.f : pdf;
	}


	bool UnitSphere::Intersect(const Ray& lcr) const {
		float e2 = Math::LengthSqr(lcr.origin);
		float d2 = Math::LengthSqr(lcr.dir);
		float de = Math::Dot(lcr.dir, lcr.origin);
		float det = de * de - d2 * (e2 - 1.f);		// delta / 4

		if (det > 0.f){
			det = Math::Sqrt(det);
			float t = -(de + det) / d2;
			if (Math::InBounds(t, lcr.t_min, lcr.t_max)){
				lcr.t_max = t;
				return true;
			}
			t = -(de - det) / d2;
			if (Math::InBounds(t, lcr.t_min, lcr.t_max)){
				lcr.t_max = t;
				return true;
			}
		}
		return false;
	}

	void UnitSphere::PostIntersect(const Ray& lcr, LocalGeo& geo) const {
		geo.normal = lcr.End();// .Normalize();
	}

	bool UnitSphere::Occlude(const Ray& lcr) const {
		float e2 = Math::LengthSqr(lcr.origin);
		float d2 = Math::LengthSqr(lcr.dir);
		float de = Math::Dot(lcr.dir, lcr.origin);
		float det = de * de - d2 * (e2 - 1.f);		// delta / 4
		if (det > 0.f){
			det = Math::Sqrt(det);
			return Math::InBounds(-(de + det) / d2, lcr.t_min, lcr.t_max) ||
				Math::InBounds(-(de - det) / d2, lcr.t_min, lcr.t_max);
		}
		return false;
	}

	float UnitSphere::Area() const { return 4 * float(Math::PI); }

	void UnitSphere::SamplePoint(const Sample *sample, Vec3 *out, Vec3 *normal) const {
		*out = Sampling::UniformSphere(sample->u, sample->v);
		*normal = *out;
	}

	void UnitSphere::SamplePoint(const Sample *sample, const Vec3& localeye, Vec3 *point, Vec3 *normal) const{
		float distSqr = Math::LengthSqr(localeye);
		// sample the sphere uniformly if eye is inside the sphere
		if (distSqr < 1.f + Ray::EPSILON)
			return SamplePoint(sample, point, normal);
		// sample sphere inside subtended cone
		float sinThetaMax2 = 1.f / distSqr;
		float cosThetaMax = Math::Sqrt(Math::Max(0.f, 1.f - sinThetaMax2));
		Ray eyeray(localeye, Sampling::UniformCone(sample->u, sample->v, cosThetaMax));
		if (!Intersect(eyeray))
			eyeray.t_max = Math::Dot(localeye, eyeray.dir);
		*normal = eyeray.End();
		*point = eyeray.End();
	}


	bool UnitPlane::Intersect(const Ray& lcr) const {
		float t = -lcr.origin.z / lcr.dir.z;
		if (Math::InBounds(t, lcr.t_min, lcr.t_max) &&
			Math::Abs(lcr.origin.x + t * lcr.dir.x) < 0.5f &&
			Math::Abs(lcr.origin.y + t * lcr.dir.y) < 0.5f) {
			lcr.t_max = t;
			return true;
		}
		return false;
	}

	void UnitPlane::PostIntersect(const Ray& lcr, LocalGeo& geo) const {
		geo.normal = Vec3::Z;// .Normalize();
	}

	bool UnitPlane::Occlude(const Ray& lcr) const {
		float t = -lcr.origin.z / lcr.dir.z;
		return Math::InBounds(t, lcr.t_min, lcr.t_max) &&
			Math::Abs(lcr.origin.x + t * lcr.dir.x) < 0.5f &&
			Math::Abs(lcr.origin.y + t * lcr.dir.y) < 0.5f;
	}

	float UnitPlane::Area() const { return 1.f; }
	float UnitPlane::Pdf(const Vec3& point) const { return 1.f; }

	void UnitPlane::SamplePoint(const Sample *sample, Vec3 *out, Vec3 *normal) const {
		out->x = Math::Lerp(sample->u, -0.5f, 0.5f);
		out->y = Math::Lerp(sample->v, -0.5f, 0.5f);
		out->z = 0.f;
		*normal = Vec3::Z;
	}
}
