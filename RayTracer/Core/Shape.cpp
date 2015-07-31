#include "stdafx.h"
#include "Math.h"
#include "Graphics/Ray.h"
#include "Intersection.h"
#include "Shape.h"
#include "Sample.h"

namespace TX{
	namespace RayTracer
	{
		float Shape::Pdf(const Ray& localwi) const {
			LocalGeo geom;
			// Test intersection between the ray and this shape which should be an area light source
			if (!Intersect(localwi)) return 0.f;
			PostIntersect(localwi, geom);
			float pdf = localwi.t_max * localwi.t_max / (Math::Abs(Dot(geom.normal, -localwi.dir)) * Area());
			return Math::IsINF(pdf) ? 0.f : pdf;
		}


		bool UnitSphere::Intersect(const Ray& lcr) const {
			float e2 = lcr.origin.LengthSqr();
			float d2 = lcr.dir.LengthSqr();
			float de = Dot(lcr.dir, lcr.origin);
			float delta_quarter = de * de - d2 * (e2 - 1.f);
			
			if (delta_quarter > 0){
				float t = -(de + Math::Sqrt(delta_quarter)) / d2;
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
			float e2 = lcr.origin.LengthSqr();
			float d2 = lcr.dir.LengthSqr();
			float de = Dot(lcr.dir, lcr.origin);
			float delta_quarter = de * de - d2 * (e2 - 1.f);
			float t = -(de + Math::Sqrt(delta_quarter)) / d2;
			return delta_quarter > 0.f && Math::InBounds(t, lcr.t_min, lcr.t_max);
		}

		void UnitSphere::SamplePoint(const Sample *sample, Vector3 *out, Vector3 *normal) const {
			*out = Sampling::UniformSphere(sample->u, sample->v);
			*normal = *out;
		}

		void UnitSphere::SamplePoint(const Sample *sample, const Vector3& localeye, Vector3 *point, Vector3 *normal) const{
			float distSqr = localeye.LengthSqr();
			// sample the sphere uniformly if eye is inside the sphere 
			if (distSqr < 1.f + Ray::EPSILON)
				return SamplePoint(sample, point, normal);
			// sample sphere inside subtended cone
			float sinThetaMax2 = 1.f / distSqr;
			float cosThetaMax = Math::Sqrt(Math::Max(0.f, 1.f - sinThetaMax2));
			Ray eyeray(localeye, Sampling::UniformCone(sample->u, sample->v, cosThetaMax));
			if (!Intersect(eyeray))
				eyeray.t_max = Dot(localeye, eyeray.dir);
			*normal = eyeray.End();
			*point = eyeray.End();
		}


		bool UnitPlane::Intersect(const Ray& lcr) const {
			float t = -lcr.origin.z / lcr.dir.z;
			Vector3 point = lcr.origin + t * lcr.dir;
			if (Math::InBounds(t, lcr.t_min, lcr.t_max) &&
				Math::InBounds(point.x, -0.5f, 0.5f) &&
				Math::InBounds(point.y, -0.5f, 0.5f)) {
				lcr.t_max = t;
				return true;
			}
			return false;
		}

		void UnitPlane::PostIntersect(const Ray& lcr, LocalGeo& geo) const {
			geo.normal = Vector3::Z;// .Normalize();
		}

		bool UnitPlane::Occlude(const Ray& lcr) const {
			float t = -lcr.origin.z / lcr.dir.z;
			return Math::InBounds(t, lcr.t_min, lcr.t_max);
		}

		void UnitPlane::SamplePoint(const Sample *sample, Vector3 *out, Vector3 *normal) const {
			out->x = Math::Lerp(sample->u, -0.5f, 0.5f);
			out->y = Math::Lerp(sample->v, -0.5f, 0.5f);
			out->z = 0.f;
			*normal = Vector3::Z;
		}
	}
}