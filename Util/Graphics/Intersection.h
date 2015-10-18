#pragma once
#include "Util.h"
#include "Math/Vector.h"
#include "Light.h"
#include "Primitive.h"

namespace TX{
	/// <summary>
	/// Basic info of a intersection.
	/// </summary>
	class Intersection {
	public:
		Intersection(){}
		inline const AreaLight *GetAreaLight() const { return prim->GetAreaLight(); }
	public:
		Ray localray;
		float dist;
		const Primitive *prim;
		uint triId;
	};

	class LocalGeo : public Intersection {
	public:
		LocalGeo(){}
		/// <summary>
		/// Gets the color of light ray emitted from this primitive (if it has an arealight attached) to the given direction.
		/// </summary>
		void Emit(const Vec3& wo, Color *out) const;

		inline void ComputeDifferentials(const Ray& ray) {
			u = Math::Normalize(Math::Cross(Math::Abs(normal.x) > 0.1f ? Vec3::Y : Vec3::X, normal));
			v = Math::Cross(normal, u);
			// TODO
		}
		inline Vec3 WorldToLocal(const Vec3& vec) const {
			assert(u != v);
			return Vec3(
				vec.x * u.x + vec.y * u.y + vec.z * u.z,
				vec.x * v.x + vec.y * v.y + vec.z * v.z,
				vec.x * normal.x + vec.y * normal.y + vec.z * normal.z);
		}

		inline Vec3 LocalToWorld(const Vec3& vec) const {
			assert(u != v);
			return vec.x * u + vec.y * v + vec.z * normal;
		}
	public:
		const BSDF *bsdf;
		Vec3 point;
		Vec3 normal;		// calculated geometry normal
		//Vec3 normal;		// from model data
		Vec3 u, v;			// local coordinate system (u, v, normal)
		//Vec3 dpdu, dpdv;		// partial derivatives at the point
		//Vec3 dndu, dndv;		// partial derivatives of the change in normal
	};
}
