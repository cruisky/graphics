#pragma once
#include "Shape.h"
#include "Math/Vector.h"
#include <vector>

namespace TX {
	class Mesh : public Shape {
	public:
		std::vector<Vec3> vertices;
		std::vector<Vec3> normals;
		std::vector<uint> triangles;
		std::vector<Vec2> uv;
	public:
		Mesh() {}
		~Mesh() {}

		inline uint VertexCount() const { return vertices.size(); }
		inline uint TriangleCount() const { return triangles.size() / 3; }

		/// <summary>
		/// Clear the vertices, normals, texcoords and triangle indices.
		/// </summary>
		virtual void Clear();

		bool Intersect(const Ray& localray) const;
		void PostIntersect(const Ray& localray, LocalGeo& geo) const;
		bool Occlude(const Ray& localray) const;
		float Area() const;
		float Pdf(const Ray& localwi) const;
		void SamplePoint(const Sample *sample, Vec3 *out, Vec3 *normal/* = nullptr*/) const;
	};
}
