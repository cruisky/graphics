#include "UtilStdAfx.h"
#include "Shape/Mesh.h"
#include "Light.h"
#include "Primitive.h"
#include "Intersection.h"
#include "Scene.h"

namespace TX
{
	Color Light::Intensity() const { return Color::WHITE; }
	Vec3 Light::Direction() const { return Vec3(0, 0, -1); }


	AreaLight::AreaLight(const Color& intensity, std::shared_ptr<Primitive> primitive, int sample_count)
		: Light(sample_count), intensity(intensity), primitive(primitive) {
		primitive->SetAreaLight(this);
	}

	void AreaLight::SampleDirect(const Vec3& eye, const Sample *lightsamples, Ray *wi, Color *lightcolor, float *pdf) const {
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
		if (!scene->Intersect(Ray(eye, dir), intxn) || primitive.get() != intxn.prim)
			return 0.f;
		return primitive->Pdf(intxn.triId, eye, dir);
	}

	Color AreaLight::Intensity() const { return intensity; }
	Vec4 AreaLight::Position() const {
		auto pos = primitive->GetMesh()->Bounds().Centroid();
		return Vec4(pos.x, pos.y, pos.z, 1);	// non-zero w means this is not a directional light
	}
}
