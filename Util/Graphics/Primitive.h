#pragma once
#include "Util.h"
#include "SceneObject.h"
#include "BSDF.h"
#include "Mesh.h"

namespace TX {
	class Primitive : public DynamicSceneObject {
	public:
		Primitive(const Mesh& mesh, std::shared_ptr<const BSDF> bsdf) : bsdf(bsdf), areaLight(nullptr){
			this->mesh = std::make_shared<Mesh>(mesh);	// copy the mesh locally
		};
		virtual ~Primitive(){}

		inline Primitive& SetAreaLight(const AreaLight *light) { areaLight = light; return *this; }
		inline const Mesh* GetMesh() const { return mesh.get(); }
		inline const BSDF* GetBSDF() const { return bsdf.get(); }
		inline const AreaLight* GetAreaLight() const { return areaLight; }

		/// <summary>
		/// Prepare this primitive for rendering, specifically
		/// - Apply transform
		/// - Generate mesh sampler if this is a light source
		/// </summary>
		Primitive& Bake();

		/// <summary>
		/// Extracts info to the LocalGeo.
		/// </summary>
		void PostIntersect(const Ray& ray, LocalGeo& geo) const;

		inline float Pdf(uint surfaceTriId, const Vec3& surfacePoint) const {
			assert(meshSampler);
			return meshSampler->Pdf(surfaceTriId, surfacePoint);
		}

		inline float Pdf(uint surfaceTriId, const Vec3& eye, const Vec3& dir) const {
			assert(meshSampler);
			Ray ray(eye, dir);
			return meshSampler->Pdf(surfaceTriId, ray);
		}

		inline void SamplePoint(const Sample *sample, Vec3 *point, uint *triId, Vec3 *normal) const {
			assert(meshSampler);
			meshSampler->SamplePoint(sample, point, triId, normal);
		}
	private:
		std::shared_ptr<const BSDF>		bsdf;
		std::shared_ptr<Mesh>			mesh;

		/// <summary>
		/// If this primitive is associated with an area light,
		/// then the mesh sampler should be created for the underlying mesh
		/// </summary>
		std::unique_ptr<MeshSampler>	meshSampler;
		const AreaLight *				areaLight;
	};
}
