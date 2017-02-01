#include "txbase/stdafx.h"
#include "mesh.h"
#include "obj.h"
#include "txbase/math/sample.h"

namespace TX {
	void Mesh::Clear() {
		vertices.clear();
		normals.clear();
		uv.clear();
		indices.clear();
		bbox_dirty_ = true;
	}

	void Mesh::ApplyTransform(const Transform& transform) {
		transform.UpdateMatrix();
		const Matrix4x4& world2local = transform.WorldToLocalMatrix();
		const Matrix4x4& local2world = transform.LocalToWorldMatrix();
		if (local2world == Matrix4x4::IDENTITY)
			return;
		for (auto vert = vertices.begin(); vert < vertices.end(); vert++) {
			*vert = Matrix4x4::TPoint(local2world, *vert);
		}
		for (auto norm = normals.begin(); norm < normals.end(); norm++) {
			*norm = Math::Normalize(Matrix4x4::TNormal(world2local, *norm));
		}
		bbox_dirty_ = true;
	}

	bool Mesh::Intersect(uint triId, const Ray& ray) const {
		// Moller-Trumbore algorithm
		const uint* idx = GetIndicesOfTriangle(triId);
		const Vec3& v0 = vertices[*idx];
		const Vec3 e1 = vertices[*(++idx)] - v0;
		const Vec3 e2 = vertices[*(++idx)] - v0;

		const Vec3 P = Math::Cross(ray.dir, e2);
		const float det = Math::Dot(e1, P);
		if (Math::Abs(det) < Ray::EPSILON)
			return false;
		const float invDet = 1.f / det;
		const Vec3 T = ray.origin - v0;
		const float u = Math::Dot(T, P) * invDet;
		if (!Math::InBounds(u, 0.f, 1.f))
			return false;
		const Vec3 Q = Math::Cross(T, e1);
		const float v = Math::Dot(ray.dir, Q) * invDet;
		if (!Math::InBounds(v, 0.f, 1.f))
			return false;

		const float t = Math::Dot(e2, Q) * invDet;

		if (!Math::InBounds(t, ray.t_min, ray.t_max))
			return false;

		ray.t_max = t;
		return true;
	}
	bool Mesh::Occlude(uint triId, const Ray& ray) const {
		const uint* idx = GetIndicesOfTriangle(triId);
		const Vec3& v0 = vertices[*idx];
		const Vec3 e1 = vertices[*(++idx)] - v0;
		const Vec3 e2 = vertices[*(++idx)] - v0;

		const Vec3 P = Math::Cross(ray.dir, e2);
		const float det = Math::Dot(e1, P);
		if (Math::Abs(det) < Ray::EPSILON)
			return false;
		const float invDet = 1 / det;
		const Vec3 T = ray.origin - v0;
		const float u = Math::Dot(T, P) * invDet;
		if (!Math::InBounds(u, 0.f, 1.f))
			return false;
		const Vec3 Q = Math::Cross(T, e1);
		const float v = Math::Dot(ray.dir, Q) * invDet;

		return Math::InBounds(Math::Dot(e2, Q) * invDet, ray.t_min, ray.t_max);
	}
	float Mesh::Area() const {
		float area = 0.f;
		for (uint i = 0; i < indices.size(); i += 3) {
			area += Area(i);
		}
		return area;
	}
	float Mesh::Area(uint triId) const {
		assert(triId % 3 == 0);
		return 0.5f * Math::Length(Math::Cross(vertices[indices[triId + 1]] - vertices[indices[triId]],
			vertices[indices[triId + 2]] - vertices[indices[triId]]));
	}
	Mesh& Mesh::LoadSphere(float radius, uint slices, uint stacks) {
		Clear();

		#pragma region vertices
		vertices.reserve((slices+1) * stacks + 2);
		vertices.push_back(Vec3::Z * radius);
		for (uint lat = 0; lat < stacks; lat++) {
			float a1 = Math::PI * float(lat + 1) / (stacks + 1);
			float rsin1 = Math::Sin(a1) * radius;
			float rcos1 = Math::Cos(a1) * radius;
			for (uint lon = 0; lon <= slices; lon++) {
				float a2 = (2.f * Math::PI) * float(lon == slices ? 0 : lon) / slices;
				float sin2 = Math::Sin(a2);
				float cos2 = Math::Cos(a2);
				//vertices[lat * (slices+1) + 1 + lon]
				vertices.emplace_back(rsin1 * cos2, rsin1 * sin2, rcos1);
			}
		}
		vertices.push_back(-Vec3::Z * radius);
		#pragma endregion

		#pragma region normals
		normals.reserve(vertices.size());
		for (auto& v : vertices) {
			normals.push_back(Math::Normalize(v));
		}
		#pragma endregion

		#pragma region uvs
		uv.reserve(vertices.size());
		uv.emplace_back(0.f, 1.f);
		for (uint lat = 0; lat < stacks; lat++)
			for (uint lon = 0; lon <= slices; lon++)
				uv.emplace_back(float(lon) / slices, 1.f - float(lat + 1) / (stacks + 1));
		uv.emplace_back(0.f, 0.f);
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

		bbox_ = BBox(Vec3(-radius), Vec3(radius));
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

		bbox_ = BBox(
			Vec3(-halfSize, -halfSize, 0),
			Vec3(halfSize, halfSize, 0));

		return *this;
	}


	MeshSampler::MeshSampler(std::shared_ptr<const Mesh> mesh) : mesh(mesh){
		sumArea = 0.f;
		const uint idxCount = mesh->indices.size();
		const uint triCount = idxCount / 3;
		assert(idxCount % 3 == 0);

		areas.reserve(triCount);
		for (uint i = 0; i < idxCount; i += 3) {
			float a = mesh->Area(i);
			areas.push_back(a);
			sumArea += a;
		}
		sumAreaRcp = 1.f / sumArea;
		areaDistro = std::make_unique<Distribution1D>(&areas[0], triCount);
	}
	void MeshSampler::SamplePoint(const Sample *sample, Vec3 *point, uint *id, Vec3 *normal) const {
		int triId = areaDistro->SampleDiscrete(sample->w, nullptr);
		float barycentricU, barycentricV;
		Sampling::UniformTriangle(sample->u, sample->v, &barycentricU, &barycentricV);
		mesh->GetPoint(triId, barycentricU, barycentricV, point, normal);
		if (id) *id = triId;
	}

	float MeshSampler::Pdf(uint triId, const Ray& wi) const {
		if (!mesh->Intersect(triId, wi))
			return 0.f;
		Vec3 normal;
		mesh->GetPoint(triId, 0.f, 0.f, nullptr, &normal);
		float pdf = wi.t_max * wi.t_max / (Math::AbsDot(normal, wi.dir) * sumArea);		// solid angle measure
		return Math::IsINF(pdf) ? 0.f : pdf;
	}
	float MeshSampler::Pdf(uint triId, const Vec3& point) const {
		return sumAreaRcp;
	}
}
