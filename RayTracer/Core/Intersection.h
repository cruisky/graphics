#pragma once
#include "fwddecl.h"

#include "Vector.h"

namespace Cruisky{
	namespace RayTracer{

		// Basic info of a intersection
		struct Intersection{
		public:
			Intersection(){}
		public:
			float dist;
			const Primitive *prim;
		};

		struct LocalGeo : Intersection {
		public:
			LocalGeo(){}
			inline void ComputeDifferentials(const Ray& ray) {
				u = Cross(Math::Abs(normal.x) > 0.1f ? Vector3::Y : Vector3::X, normal).Normalize();
				v = Cross(normal, u);
				// TODO
			}
			inline Vector3 WorldToLocal(const Vector3& vec) const {
				assert(u != v);
				return Vector3(
					vec.x * u.x + vec.y * u.y + vec.z * u.z,
					vec.x * v.x + vec.y * v.y + vec.z * v.z,
					vec.x * normal.x + vec.y * normal.y + vec.z * normal.z);
			}

			inline Vector3 LocalToWorld(const Vector3& vec) const {
				assert(u != v);
				return vec.x * u + vec.y * v + vec.z * normal;
			}
		public:
			const BSDF *bsdf;
			Vector3 point;
			Vector3 normal;			// from surface
			//Vector3 normal;		// from normal map
			Vector3 u, v;			// local coordinate system (u, v, normal)
			//Vector3 dpdu, dpdv;		// partial derivatives at the point
			//Vector3 dndu, dndv;		// partial derivatives of the change in normal
		};
	}
}