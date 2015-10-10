#pragma once
#include "Util.h"
#include "Graphics/Color.h"
#include "SceneObject.h"

namespace TX {
	class Light : public SceneObject {
	public:
		Light(int sample_count = 1) : sample_count(sample_count){}
		virtual ~Light(){};

		// Emits a light ray from point to the light source.
		virtual void Illuminate(const Vec3& pos, const Sample *lightsamples, Ray *wi, Color *lightcolor, float *pdf) const = 0;
		virtual float Pdf(const Vec3& pos, const Vec3& dir) const = 0;

		// Indicate whether this light is described by a delta distribution, if so, this light cannot be randomly sampled
		virtual bool IsDelta() const = 0;
	public:
		const int sample_count;
	};
		

	class AreaLight : public Light {
	public:
		AreaLight(const Color& intensity, Primitive *primitive, int sample_count = 1);
		inline bool IsDelta() const { return false; }
		virtual void Illuminate(const Vec3& pos, const Sample *lightsamples, Ray *wi, Color *lightcolor, float *pdf) const;
		virtual void Emit(const Vec3& pos, const Vec3& normal, const Vec3& wo, Color *out) const;
		virtual float Pdf(const Vec3& pos, const Vec3& dir) const;
	public:
		Color intensity;
		const Primitive * const primitive;
	};
}