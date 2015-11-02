#pragma once
#include "Util.h"
#include "SceneObject.h"
#include <memory>
#include "BSDF.h"
#include "Mesh.h"

namespace TX {
	class Primitive : public SceneObject {
		// Allows an AreaLight to bind with this primitive at its creation.
		friend class AreaLight;
	public:
		Primitive(std::shared_ptr<const Mesh> meshes, std::shared_ptr<const BSDF> bsdf) : bsdf_(bsdf), mesh_(meshes), areaLight(nullptr){};
		virtual ~Primitive(){}

		///// <summary>
		///// Checks if the global ray intersects this primitive, if so
		///// fills dist and prim in the intersection,
		///// and modify the length (t_max) of the ray.
		///// The dir of the global ray should be normalized.
		///// </summary>
		//bool Intersect(const Ray& nRay, Intersection& intersection) const;
		/// <summary>
		/// Extracts info to the LocalGeo.
		/// </summary>
		void PostIntersect(const Ray& ray, LocalGeo& geo) const;
		///// <summary>
		///// Test if this object occludes the ray.
		///// </summary>
		//bool Occlude(const Ray& ray) const;

		inline float Pdf(uint surfaceTriId, const Vec3& surfacePoint) const {
			assert(meshSampler);
			//Vec3 localPoint = Matrix4x4::TPoint(transform.WorldToLocalMatrix(), surfacePoint.pos);
			//return meshSampler->Pdf(surfacePoint.surfaceTriId, localPoint);
			return meshSampler->Pdf(0, Vec3::ZERO);		// it doesn't care what the paramters are
		}

		inline float Pdf(uint surfaceTriId, const Vec3& eye, const Vec3& dir) const {
			assert(meshSampler);
			Ray localray;
			localray.Reset(eye, dir);
			transform.ToLocal(localray);
			localray.dir = Math::Normalize(localray.dir);		// normalize the local ray so that after intersection, t value will be the local distance
			return meshSampler->Pdf(surfaceTriId, localray);
		}

		inline void SamplePoint(const Sample *sample, Vec3 *point, uint *triId, Vec3 *normal) const {
			assert(meshSampler);
			meshSampler->SamplePoint(sample, point, triId, normal);
			*point = Matrix4x4::TPoint(transform.LocalToWorldMatrix(), *point);
			if (normal)
				*normal = Matrix4x4::TNormal(transform.WorldToLocalMatrix(), *normal);	// use inverse when transforming normal
		}

		const Mesh* GetMesh() const;
		const BSDF* GetBSDF() const;
		const AreaLight* GetAreaLight() const;
	private:
		std::shared_ptr<const BSDF>			bsdf_;
		std::shared_ptr<const Mesh>			mesh_;

		/// <summary>
		/// If this primitive is associated with an area light,
		/// then the mesh sampler should be created for the underlying mesh
		/// </summary>
		std::shared_ptr<MeshSampler>		meshSampler;
		const AreaLight *					areaLight;
	};
}
