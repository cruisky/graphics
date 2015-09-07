#pragma once
#include "fwddecl.h"

#include "Graphics/Color.h"
#include "Graphics/Ray.h"

namespace TX{
	namespace RayTracer{
		class Tracer {
		public:
			Tracer(int maxdepth = 5) : maxdepth_(maxdepth){}
			virtual ~Tracer(){}

			inline void Trace(const Scene *scene, const Ray& ray, const CameraSample& samples, RNG& rng, Color *color){
				rng_ = &rng;
				*color = Li(scene, ray, maxdepth_, samples);
				//color->Clamp();
			}

			virtual void BakeSamples(const Scene *scene, const CameraSample *samples) = 0;
		protected:
			// The recursive tracing function
			virtual Color Li(const Scene *scene, const Ray& ray, int depth, const CameraSample& samplebuf) = 0;
			// Pick necessary samples from current sample buffer for future use
			Color TraceDirectLight(const Scene *scene, const Ray& ray, const LocalGeo& geom, const Light *light, const Sample *lightsample, const Sample *bsdfsample);
			Color TraceSpecularReflect(const Scene *scene, const Ray& ray, const LocalGeo& geom, int depth, const CameraSample& samplebuf);
			Color TraceSpecularTransmit(const Scene *scene, const Ray& ray, const LocalGeo& geom, int depth, const CameraSample& samplebuf);
		protected:
			const int maxdepth_;
			RNG *rng_;
		};
	}
}
