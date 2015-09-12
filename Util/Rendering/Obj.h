#pragma once
#include "RenderingFwd.h"
#include <vector>
#include "Graphics/Color.h"
#include "Math/Vector.h"

namespace TX {
	namespace Rendering{
		struct ObjMaterial {
			Color			ambient;
			Color			diffuse;
			Color			specular;
			Color			transmittance;
			Color			emission;
			float			shininess;
			float			ior;
			float			dissolve;

			std::string		name;
			std::string		texAmbient;
			std::string		texDiffuse;
			std::string		texSpecular;
			std::string		texSpecularHighlight;
			std::string		texBump;
			std::string		texDisplacement;
			std::string		texAlpha;

			ObjMaterial(const char *name = "") :
				name(name),
				ambient(0.75f, 0.75f, 0.75f),
				specular(0.f, 0.f, 0.f),
				transmittance(0.f, 0.f, 0.f),
				emission(0.f, 0.f, 0.f),
				shininess(0.f),
				ior(0.f),
				dissolve(1.f){}
		};

		struct ObjMesh {
			std::vector<Vector3>		vertices;
			std::vector<Vector3>		normals;
			std::vector<Vector2>		uvs;
			std::vector<uint>			indices;
			std::vector<uint>			materials;

			void LoadQuad(
				float length = 1.f,
				const Vector3& position = Vector3::ZERO,
				const Vector3& rotation = Vector3::ZERO,
				const Vector3& scale = Vector3::ONE);
			void LoadSphere(
				float radius,
				const Vector3& position = Vector3::ZERO,
				const Vector3& rotation = Vector3::ZERO,
				const Vector3& scale = Vector3::ONE);
			void Clear();

			void PostIntersect(const Ray& ray, LocalGeo& geom) const;

			const uint* GetIndexOfTriangle(uint triIdx) const;

			inline int TriangleCount() const { return indices.size() / 3; }
		};

		struct ObjShape{
			std::string		name;
			ObjMesh			mesh;

			ObjShape(const char *name = "") :
				name(name){}
		};

		void LoadObj(
			std::vector<ObjShape>& objects,
			std::vector<ObjMaterial>& materials,
			const char *objFile, const char *mtlDir = nullptr);
	}
}