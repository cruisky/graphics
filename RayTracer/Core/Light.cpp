#include "stdafx.h"
#include "Light.h"

#include "Primitive.h"

namespace Cruisky
{
	namespace RayTracer {
		AreaLight::AreaLight(const Color& intensity, Primitive *primitive, int sample_count)
			: Light(sample_count), intensity(intensity), primitive(primitive) {
			assert(primitive);
			primitive->area_light_ = this;
		}

		void AreaLight::Illuminate(const Vector3& pos, const Sample *lightsamples, Ray *wi, Color *lightcolor, float *pdf) const {
			static Vector3 normal;
			static Vector3 lightpoint;
			primitive->SamplePoint(lightsamples, &lightpoint, &normal);
			wi->SetSegment(pos, lightpoint);
			*pdf = Pdf(pos, wi->dir);
			Emit(lightpoint, normal, -wi->dir, lightcolor);
		}
		void AreaLight::Emit(const Vector3& pos, const Vector3& normal, const Vector3& wo, Color *out) const {
			*out = Dot(normal, wo) > 0.f ? intensity : 0.f;
		}

		float AreaLight::Pdf(const Vector3& pos, const Vector3& dir) const {
			return primitive->Pdf(pos, dir);
		}
	}
}