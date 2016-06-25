#pragma once
#include "Util.h"
#include "Graphics/Color.h"
#include "Math/Sample.h"
#include "SceneObject.h"

namespace TX {
	class Light : public SceneObject {
	public:
		Light(int sample_count = 1) : sample_count(sample_count){}
		virtual ~Light(){};

		/// <summary>
		/// Emits a light ray from point to the light source.
		/// </summary>
		/// <param name="pos">The point where the light ray is pointing to</param>
		/// <param name="lightsamples">Sample</param>
		/// <param name="wi">Emitted camera ray</param>
		/// <param name="lightcolor">Light color</param>
		/// <param name="pdf">Pdf</param>
		virtual void SampleDirect(const Vec3& pos, const Sample *lightsamples, Ray *wi, Color *lightcolor, float *pdf) const = 0;
		virtual float Pdf(const Vec3& eye, const Vec3& dir) const = 0;

		/// <summary>
		/// Indicate whether this light is described by a delta distribution (if it is, it cannot be randomly sampled)
		/// </summary>
		virtual bool IsDelta() const = 0;

		virtual Color Intensity() const;
		virtual Vec4 Position() const = 0;
		virtual Vec3 Direction() const;

	public:
		const int sample_count;
	};

	class AreaLight : public Light {
	public:
		AreaLight(const Color& intensity, std::shared_ptr<Primitive> primitive, int sample_count = 1);
		inline bool IsDelta() const { return false; }
		virtual void SampleDirect(const Vec3& pos, const Sample *lightsamples, Ray *wi, Color *lightcolor, float *pdf) const;
		virtual void Emit(const Vec3& pos, const Vec3& normal, const Vec3& wo, Color *out) const;
		virtual float Pdf(const Vec3& eye, const Vec3& dir) const;

		virtual Color Intensity() const;
		virtual Vec4 Position() const;
	public:
		Color intensity;
		std::shared_ptr<const Primitive> primitive;
	};
}
