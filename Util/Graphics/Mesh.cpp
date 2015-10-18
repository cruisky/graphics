#include "UtilStdAfx.h"
#include "Mesh.h"
#include "Intersection.h"
#include "Obj.h"

namespace TX {
	void Mesh::Clear() {
		vertices.clear();
		normals.clear();
		uv.clear();
		triangles.clear();
	}

	bool Mesh::Intersect(const Ray& localray) const {
		//TODO
		return false;
	}
	void Mesh::PostIntersect(const Ray& localray, LocalGeo& geom) const {
		const uint vId1 = 3 * geom.triId;
		const uint vId2 = vId1 + 1;
		const uint vId3 = vId1 + 2;

		const Vec3& vert1 = vertices[vId1];
		const Vec3& vert2 = vertices[vId2];
		const Vec3& vert3 = vertices[vId3];

		//const Vec3& norm1 = normals[vId1];
		//const Vec3& norm2 = normals[vId2];
		//const Vec3& norm3 = normals[vId3];

		geom.normal = Math::Cross(vert2 - vert1, vert3 - vert1);
	}
	bool Mesh::Occlude(const Ray& localray) const {
		//TODO
		return false;
	}
	float Mesh::Area() const {
		float area = 0.f;
		for (auto i = 0U; i < triangles.size(); i += 3) {
			area += 0.5f * Math::Length(Math::Cross(vertices[triangles[i + 1]] - vertices[triangles[i]],
				vertices[triangles[i + 2]] - vertices[triangles[i]]));
		}
		return area;
	}
	float Mesh::Pdf(const Ray& localwi) const {
		//TODO
		return 0.f;
	}
	void Mesh::SamplePoint(const Sample *sample, Vec3 *out, Vec3 *normal/* = nullptr*/) const {
		//TODO
	}
}
