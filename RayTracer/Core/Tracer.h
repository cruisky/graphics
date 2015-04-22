#pragma once
#include "fwddecl.h"

#include "Color.h"
#include "Ray.h"

namespace Cruisky{
	namespace RayTracer{
		class Tracer {
		public:
			Tracer(int maxdepth = 5) : maxdepth_(maxdepth){}
			virtual ~Tracer(){}

			inline void Trace(const Ray& ray, const CameraSample& samples, Color *color){
				samples_ = &samples;
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
			Color TraceReflection(const Ray& ray, const LocalGeo& geo, float reflectivity, int depth);
			Color TraceRefraction(const Ray& ray, const LocalGeo& geo, int depth);
			Color TraceDirectLight(const Ray& ray, const LocalGeo& geo, const Light *light, const Sample *lightsample, const Sample *bsdfsample);

		protected:
			int maxdepth_;
			const Scene *scene_;
			const CameraSample *samples_;
		};
	}
}