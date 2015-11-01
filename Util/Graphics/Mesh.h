#pragma once
#include "Shape.h"
#include "Math/Vector.h"
#include <vector>

namespace TX {
	class Mesh : public Shape {
	public:
		std::vector<Vec3> vertices;
		std::vector<Vec3> normals;
		std::vector<uint> indices;
		std::vector<Vec2> uv;
	public:
		Mesh() {}
		~Mesh() {}

		inline uint VertexCount() const { return vertices.size(); }
		inline uint IndexCount() const { return indices.size(); }
		inline uint TriangleCount() const { return indices.size() / 3; }

		/// <summary>
		/// Clear the vertices, normals, texcoords and triangle indices.
		/// </summary>
		virtual void Clear();
		/// <summary>
		/// Load a sphere to this mesh.
		/// </summary>
		/// <param name="radius"> Radius of the sphere </param>
		/// <param name="slices"> Number of slices (meridian) </param>
		/// <param name="stacks"> Number of stacks (ring of latitude on xz plane) </param>
		Mesh& LoadSphere(float radius = 1.f, uint slices = 64, uint stacks = 64);
		/// <summary>
		/// Load a plane to this mesh.
		/// The plane faces towards z-axis.
		/// </summary>
		/// <param name="size"> Edge size </param>
		Mesh& LoadPlane(float size = 1.f);

		bool Intersect(const Ray& localray) const;
		void PostIntersect(const Ray& localray, LocalGeo& geo) const;
		bool Occlude(const Ray& localray) const;
		float Area() const;
		float Pdf(const Ray& localwi) const;
		void SamplePoint(const Sample *sample, Vec3 *out, Vec3 *normal/* = nullptr*/) const;
	};
}
