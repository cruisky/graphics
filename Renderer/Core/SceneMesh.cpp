#include "stdafx.h"
#include "SceneMesh.h"
#include "Intersection.h"

namespace TX {
	void SceneMesh::PostIntersect(LocalGeo& geom) const {
		const uint* idx = GetIndicesOfTriangle(geom.triId);

		const Vec3& vert1 = vertices[idx[0]];
		const Vec3& vert2 = vertices[idx[1]];
		const Vec3& vert3 = vertices[idx[2]];

		//geom.normal = Math::Normalize(Math::Cross(vert2 - vert1, vert3 - vert1));

		const Vec3& norm1 = normals[idx[0]];
		const Vec3& norm2 = normals[idx[1]];
		const Vec3& norm3 = normals[idx[2]];
		geom.normal = norm1 * (1.f - geom.uv.u - geom.uv.v) +
			norm2 * geom.uv.u +
			norm3 * geom.uv.v;
	}
}
