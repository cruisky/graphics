#include "UtilStdAfx.h"
#include "Light.h"
#include "Primitive.h"
#include "Mesh.h"
#include "Intersection.h"
#include "Scene.h"

namespace TX
{
	AreaLight::AreaLight(const Color& intensity, Primitive *primitive, std::shared_ptr<MeshSampler> sampler, int sample_count)
		: Light(sample_count), intensity(intensity), primitive(primitive) {
		assert(primitive);
		assert(primitive->GetMesh() == sampler->mesh.get());
		primitive->areaLight = this;
		primitive->meshSampler = sampler;
	}

	void AreaLight::Illuminate(const Vec3& eye, const Sample *lightsamples, Ray *wi, Color *lightcolor, float *pdf) const {
		Vec3 normal;
		Vec3 lightpoint;
		uint triId;

		// take a random point on the surface
		primitive->SamplePoint(lightsamples, &lightpoint, &triId, &normal);
		wi->SetSegment(eye, lightpoint);
		// compute pdf & color of the ray
		*pdf = primitive->Pdf(triId, eye, wi->dir);
		Emit(lightpoint, normal, -wi->dir, lightcolor);
	}
	void AreaLight::Emit(const Vec3& pos, const Vec3& normal, const Vec3& wo, Color *out) const {
		*out = Math::Dot(normal, wo) > 0.f ? intensity : 0.f;
	}

	float AreaLight::Pdf(const Vec3& eye, const Vec3& dir) const {
		Intersection intxn;
		if (!scene_->Intersect(Ray(eye, dir), intxn) || intxn.prim != primitive)
			return 0.f;
		return primitive->Pdf(intxn.triId, eye, dir);
	}
}
