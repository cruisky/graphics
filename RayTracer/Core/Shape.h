#pragma once
#include "fwddecl.h"

namespace TX {
	namespace RayTracer
	{
		class Shape {
		public:
			virtual ~Shape(){};
			// Check if ray intersects the shape, if so modifies the 
			// length(t_max) of the ray.
			virtual bool Intersect(const Ray& localray) const = 0;
			// computes surface normal without normalizing
			virtual void PostIntersect(const Ray& localray, LocalGeo& geo) const = 0;
			// Only checks intersection.
			virtual bool Occlude(const Ray& localray) const = 0;
			
			// Surface area.
			virtual float Area() const = 0;
			// Pdf of a point at this shape.
			virtual float Pdf(const Vector3& localpoint) const { return 1.f / Area(); }
			// Pdf of a ray emitting from this shape.
			virtual float Pdf(const Ray& localwi) const;
			// General sampling.
			virtual void SamplePoint(const Sample *sample, Vector3 *out, Vector3 *normal/* = nullptr*/) const = 0;
			// Samples only from points that are visible from the eye.
			virtual void SamplePoint(const Sample *sample, const Vector3& localeye, Vector3 *out, Vector3 *normal) const{
				SamplePoint(sample, out, normal);
			}
		};

		class UnitSphere : public Shape {
		public:
			UnitSphere(){}
			bool Intersect(const Ray& localray) const;
			void PostIntersect(const Ray& localray, LocalGeo& geo) const;
			bool Occlude(const Ray& localray) const;

			float Area() const { return Math::FOUR_PI; }
			float Pdf(const Vector3& point) const { return Math::FOUR_PI_RCP; }
			void SamplePoint(const Sample *sample, Vector3 *out, Vector3 *normal) const;
			void SamplePoint(const Sample *sample, const Vector3& localeye, Vector3 *out, Vector3 *normal) const;

		};

		class UnitPlane : public Shape {
		public:
			UnitPlane(){}
			bool Intersect(const Ray& localray) const;
			void PostIntersect(const Ray& localray, LocalGeo& geo) const;
			bool Occlude(const Ray& localray) const;

			float Area() const { return 1.f; }
			float Pdf(const Vector3& point) const { return 1.f; }
			void SamplePoint(const Sample *sample, Vector3 *out, Vector3 *normal) const;
		};
	}
}