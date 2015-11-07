#pragma once

#include "Util.h"
#include "SSE/SSE.h"
#include "Graphics/Intersection.h"
#include "Graphics/Primitive.h"		// temp
namespace TX {
	struct BuildVertex {
		Vec3 pos;
		float pad;
		BuildVertex(Vec3 v) : pos(v) {}
		BuildVertex(float x = 0.f, float y = 0.f, float z = 0.f) :
			pos(x, y, z) {}
	};

	struct BuildTri {
		uint idx0, idx1, idx2;
		uint primId;		// id of the primitive
		uint triId;			// id of the triangle in the primitive
		BuildTri() {}
		BuildTri(uint idx0, uint idx1, uint idx2, uint primId, uint triId) :
			idx0(idx0),
			idx1(idx1),
			idx2(idx2),
			primId(primId),
			triId(triId){
		}
	};
	/// <summary>
	/// Packed four triangles for faster intersection.
	/// </summary>
	class Tri4 {
	private:
		SSE::Vec3V4F vert0;
		SSE::Vec3V4F edge1;
		SSE::Vec3V4F edge2;
		SSE::Vec3V4F normal;

		Vec<4, uint> primId;
		Vec<4, uint> triId;
	public:
		Tri4() {}
		/// <summary>
		/// Initialize triangle data.
		/// </summary>
		inline void Pack(const BuildVertex *verts[4][3], const BuildTri *prims[4], uint count) {
			assert(count <= 4);
			for (uint i = 0; i < count; i++) {
				vert0.x[i] = verts[i][0]->pos.x;
				vert0.y[i] = verts[i][0]->pos.y;
				vert0.z[i] = verts[i][0]->pos.z;

				Vec3 e1 = verts[i][1]->pos - verts[i][0]->pos;
				Vec3 e2 = verts[i][2]->pos - verts[i][0]->pos;

				edge1.x[i] = e1.x;
				edge1.y[i] = e1.y;
				edge1.z[i] = e1.z;
				edge2.x[i] = e2.x;
				edge2.y[i] = e2.y;
				edge2.z[i] = e2.z;

				Vec3 norm = Math::Cross(e1, e2);
				normal.x[i] = norm.x;
				normal.y[i] = norm.y;
				normal.z[i] = norm.z;

				primId[i] = prims[i]->primId;
				triId[i] = prims[i]->triId;
			}
			for (uint i = count; i < 4; i++) {
				vert0.x[i]
					= vert0.y[i]
					= vert0.z[i]
					= edge1.x[i]
					= edge1.y[i]
					= edge1.z[i]
					= edge2.x[i]
					= edge2.y[i]
					= edge2.z[i] = 0.f;
				primId[i] = triId[i] = -1;
			}
		}

		inline bool Intersect(const Ray& ray, Intersection& intxn, const std::vector<std::shared_ptr<Primitive>>& prims) const {
			// Moller-Trumbore algorithm
			const SSE::Vec3V4F origin(ray.origin);
			const SSE::Vec3V4F dir(ray.dir);
			const SSE::Vec3V4F P = Math::Cross(dir, edge2);
			const SSE::V4Float det = Math::Dot(edge1, P);
			SSE::V4Bool valid = (det != SSE::V4Float::ZERO);
			if (SSE::None(valid))
				return false;

			const SSE::V4Float invDet = SSE::V4Float::ONE / det;

			const SSE::Vec3V4F T = origin - vert0;
			const SSE::V4Float u = Math::Dot(T, P) * invDet;
			valid &= (u > SSE::V4Float::ZERO) & (u < SSE::V4Float::ONE);
			if (None(valid))
				return false;

			const SSE::Vec3V4F Q = Math::Cross(T, edge1);
			const SSE::V4Float v = Math::Dot(dir, Q) * invDet;
			valid &= (v > SSE::V4Float::ZERO) & (u + v < SSE::V4Float::ONE);
			if (None(valid))
				return false;

			const SSE::V4Float t = Math::Dot(edge2, Q) * invDet;
			valid &= (t > SSE::V4Float(ray.t_min)) & (t < SSE::V4Float(ray.t_max));
			if (None(valid))
				return false;

			const size_t idx = SSE::SelectMin(valid, t);
			float rayT = t[idx];
			float barycentricU = u[idx], barycentricV = v[idx];

			// update intersection and ray length
			ray.t_max = rayT;
			intxn.dist = rayT;
			intxn.uv.u = barycentricU;
			intxn.uv.v = barycentricV;
			intxn.prim = prims[primId[idx]].get();
			intxn.triId = triId[idx];
			return true;
		}
		inline bool Occlude(const Ray& ray) const {
			const SSE::Vec3V4F origin(ray.origin);
			const SSE::Vec3V4F dir(ray.dir);
			const SSE::Vec3V4F P = Math::Cross(dir, edge2);
			const SSE::V4Float det = Math::Dot(edge1, P);
			SSE::V4Bool valid = (det != SSE::V4Float::ZERO);
			if (SSE::None(valid))
				return false;

			const SSE::V4Float invDet = SSE::V4Float::ONE / det;

			const SSE::Vec3V4F T = origin - vert0;
			const SSE::V4Float u = Math::Dot(T, P) * invDet;
			valid &= (u > SSE::V4Float::ZERO) & (u < SSE::V4Float::ONE);
			if (SSE::None(valid))
				return false;

			const SSE::Vec3V4F Q = Math::Cross(T, edge1);
			const SSE::V4Float v = Math::Dot(dir, Q) * invDet;
			valid &= (v > SSE::V4Float::ZERO) & (u + v < SSE::V4Float::ONE);
			if (SSE::None(valid))
				return false;

			const SSE::V4Float t = Math::Dot(edge2, Q) * invDet;
			const size_t idx = SSE::SelectMin(valid, t);
			float rayT = t[idx];
			return Math::InBounds(rayT, ray.t_min, ray.t_max);
		}
	};
}
