#include "UtilStdAfx.h"
#include "Mesh.h"
#include "Intersection.h"
#include "Obj.h"

namespace TX {
	void Mesh::Clear() {
		vertices.clear();
		normals.clear();
		uv.clear();
		indices.clear();
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
		for (auto i = 0U; i < indices.size(); i += 3) {
			area += 0.5f * Math::Length(Math::Cross(vertices[indices[i + 1]] - vertices[indices[i]],
				vertices[indices[i + 2]] - vertices[indices[i]]));
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

	Mesh& Mesh::LoadSphere(float radius, uint slices, uint stacks) {
		Clear();

		#pragma region vertices
		vertices.reserve((slices+1) * stacks + 2);
		vertices.push_back(Vec3::UP * radius);
		for (uint lat = 0; lat < stacks; lat++) {
			float a1 = Math::PI * float(lat + 1) / (stacks + 1);
			float rsin1 = Math::Sin(a1) * radius;
			float rcos1 = Math::Cos(a1) * radius;
			for (uint lon = 0; lon <= slices; lon++) {
				float a2 = (2.f * Math::PI) * float(lon == slices ? 0 : lon) / slices;
				float sin2 = Math::Sin(a2);
				float cos2 = Math::Cos(a2);
				//vertices[lat * (slices+1) + 1 + lon]
				vertices.emplace_back(rsin1 * cos2, rcos1, rsin1 * sin2);
			}
		}
		vertices.push_back(Vec3::DOWN * radius);
		#pragma endregion

		#pragma region normals
		normals.reserve(vertices.size());
		for (auto& v : vertices) {
			normals.push_back(Math::Normalize(v));
		}
		#pragma endregion

		#pragma region uvs
		uv.reserve(vertices.size());
		uv.push_back(Vec2::Y);
		for (uint lat = 0; lat < stacks; lat++)
			for (uint lon = 0; lon <= slices; lon++)
				uv.emplace_back(float(lon) / slices, 1.f - float(lat + 1) / (stacks + 1));
		uv.push_back(Vec2::ZERO);
		#pragma endregion

		#pragma region indices
		const uint faceCount = vertices.size();
		const uint triCount = faceCount * 2;
		const uint indexCount = triCount * 3;
		indices.reserve(indexCount);

		// top cap
		for (uint lon = 0; lon < slices; lon++) {
			indices.push_back(0);
			indices.push_back(lon + 1);
			indices.push_back(lon + 2);
		}

		// middle
		for (uint lat = 0; lat < stacks - 1; lat++) {
			for (uint lon = 0; lon < slices; lon++) {
				uint curr = lon + lat  * (slices + 1) + 1;
				uint next = curr + slices + 1;

				indices.push_back(curr);
				indices.push_back(next);
				indices.push_back(next + 1);

				indices.push_back(curr);
				indices.push_back(next + 1);
				indices.push_back(curr + 1);
			}
		}

		// bottom cap
		for (uint lon = 0; lon < slices; lon++) {
			indices.push_back(vertices.size() - 1);
			indices.push_back(vertices.size() - lon - 2);
			indices.push_back(vertices.size() - lon - 3);
		}
		#pragma endregion

		return *this;
	}

	Mesh& Mesh::LoadPlane(float size) {
		Clear();
		const float halfSize = size * 0.5f;

		vertices.reserve(4);
		vertices.emplace_back(-halfSize, -halfSize, 0.f);
		vertices.emplace_back(+halfSize, -halfSize, 0.f);
		vertices.emplace_back(+halfSize, +halfSize, 0.f);
		vertices.emplace_back(-halfSize, +halfSize, 0.f);

		normals.reserve(4);
		for (int i = 0; i < 4; i++)
			normals.push_back(Vec3::Z);

		uv.reserve(4);
		uv.emplace_back(-1.f, -1.f);
		uv.emplace_back(+1.f, -1.f);
		uv.emplace_back(+1.f, +1.f);
		uv.emplace_back(-1.f, +1.f);

		indices.reserve(6);
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);

		return *this;
	}
}
