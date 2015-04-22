#pragma once
#include "fwddecl.h"

#include "SceneObject.h"

namespace Cruisky {
	namespace RayTracer {

		class VisibilityTester {
		public:
			void SetSegment(const Vector3& origin, const Vector3& dest);
			bool Unocclude(const Scene *scene) const;
		private:
			Ray ray_;
		};

		class Light : public SceneObject {
		public:
			Light(int sample_count) : sample_count(sample_count){}
			virtual ~Light(){};

			// Emits a light ray from point to the light source.
			virtual void Emit(const LocalGeo& geo, Ray *out, Color *lightcolor) const = 0;
			//virtual Color Sample(const Vector3& pos, const Sample& light_sample, Vector3 *wi, Color *lightcolor) const = 0;
		public:
			const int sample_count;
		};

		
	}
}