#pragma once
#include "Util.h"
namespace TX {
	class Shape {
	public:
		virtual ~Shape(){};
		/// <summary>
		/// Check if ray intersects the shape, if so modifies the
		/// length(t_max) of the ray.
		/// </summary>
		virtual bool Intersect(const Ray& localray) const = 0;
		/// <summary>
		/// Computes surface normal without normalizing.
		/// </summary>
		virtual void PostIntersect(const Ray& localray, LocalGeo& geo) const = 0;
		/// <summary>
		/// Test if the ray is occluded by this object.
		/// </summary>
		virtual bool Occlude(const Ray& localray) const = 0;
		/// <summary>
		/// Surface area.
		/// </summary>
		virtual float Area() const = 0;
		/// <summary>
		/// Pdf of a point on this shape.
		/// </summary>
		virtual float Pdf(const Vec3& localpoint) const;
		/// <summary>
		/// Pdf of a ray emitting from this shape.
		/// </summary>
		virtual float Pdf(const Ray& localwi) const;
		/// <summary>
		/// General sampling.
		/// </summary>
		virtual void SamplePoint(const Sample *sample, Vec3 *out, Vec3 *normal/* = nullptr*/) const = 0;
		/// <summary>
		/// Samples only from points that are visible from the eye.
		/// </summary>
		virtual void SamplePoint(const Sample *sample, const Vec3& localeye, Vec3 *out, Vec3 *normal) const{
			SamplePoint(sample, out, normal);
		}
	};

	class UnitSphere : public Shape {
	public:
		UnitSphere(){}
		bool Intersect(const Ray& localray) const;
		void PostIntersect(const Ray& localray, LocalGeo& geo) const;
		bool Occlude(const Ray& localray) const;

		float Area() const;
		void SamplePoint(const Sample *sample, Vec3 *out, Vec3 *normal) const;
		void SamplePoint(const Sample *sample, const Vec3& localeye, Vec3 *out, Vec3 *normal) const;

	};

	class UnitPlane : public Shape {
	public:
		/// <summary>
		/// Constructs a unit plane on xy plane.
		/// </summary>
		UnitPlane(){}
		bool Intersect(const Ray& localray) const;
		void PostIntersect(const Ray& localray, LocalGeo& geo) const;
		bool Occlude(const Ray& localray) const;

		float Area() const;
		float Pdf(const Vec3& point) const;
		void SamplePoint(const Sample *sample, Vec3 *out, Vec3 *normal) const;
	};
}
