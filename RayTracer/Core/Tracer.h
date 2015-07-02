#pragma once
#include "fwddecl.h"

#include "Color.h"
#include "Ray.h"

namespace TX{
	namespace RayTracer{
		class Tracer {
		public:
			Tracer(int maxdepth = 5) : maxdepth_(maxdepth){}
			virtual ~Tracer(){}

			inline void Trace(const Ray& ray, const CameraSample& samples, RNG& rng, Color *color){
				samples_ = &samples;
				rng_ = &rng;
				ProcessSamples();
				*color = Li(ray, maxdepth_);
				color->Clamp();
			}

			inline void SetScene(const Scene *scene){ scene_ = scene; }

		protected:
			// The recursive tracing function
			virtual Color Li(const Ray& ray, int depth) = 0;
			// Pick necessary samples from current sample buffer for future use
			virtual void ProcessSamples() = 0;
			Color TraceDirectLight(const Ray& ray, const LocalGeo& geom, const Light *light, const Sample *lightsample, const Sample *bsdfsample);
			Color TraceSpecularReflect(const Ray& ray, const LocalGeo& geom, int depth);
			Color TraceSpecularTransmit(const Ray& ray, const LocalGeo& geom, int depth);
		protected:
			const int maxdepth_;
			const Scene *scene_;
			const CameraSample *samples_;
			RNG *rng_;
		};
	}
}