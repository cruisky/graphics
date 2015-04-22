#pragma once
#include "fwddecl.h"

#include "SceneObject.h"

namespace Cruisky {
	namespace RayTracer {
		class Light : public SceneObject {
		public:
			Light(int sample_count) : sample_count(sample_count){}
			virtual ~Light(){};

			// Emits a light ray from point to the light source.
			virtual void Illuminate(const Vector3& pos, const Sample *lightsamples, Ray *out, Color *lightcolor, float *pdf) const = 0;
			virtual void Emit(const Vector3& dir, Color *out) const = 0;
			virtual float Pdf(const Vector3& pos, const Vector3& dir) const = 0;
		public:
			const int sample_count;
		};
		
	}
}