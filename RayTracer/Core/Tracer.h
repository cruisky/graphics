#pragma once
#include "fwddecl.h"

#include "Graphics/Color.h"
#include "Graphics/Ray.h"

namespace TX{
	namespace RayTracer{
		class Tracer {
		public:
			Tracer(const Scene *scene, int maxdepth = 5) : scene_(scene), maxdepth_(maxdepth){}
			virtual ~Tracer(){}

			inline void Trace(const Ray& ray, const CameraSample& samples, RNG& rng, Color *color){
				rng_ = &rng;
				*color = Li(ray, maxdepth_, samples);
				color->Clamp();
			}

			virtual void BakeSamples(const Scene *scene, const CameraSample *samples) = 0;
		protected:
			// The recursive tracing function
			virtual Color Li(const Ray& ray, int depth, const CameraSample& samplebuf) = 0;
			// Pick necessary samples from current sample buffer for future use
			Color TraceDirectLight(const Ray& ray, const LocalGeo& geom, const Light *light, const Sample *lightsample, const Sample *bsdfsample);
			Color TraceSpecularReflect(const Ray& ray, const LocalGeo& geom, int depth, const CameraSample& samplebuf);
			Color TraceSpecularTransmit(const Ray& ray, const LocalGeo& geom, int depth, const CameraSample& samplebuf);
		protected:
			const int maxdepth_;
			const Scene *scene_;
			RNG *rng_;
		};
	}
}
