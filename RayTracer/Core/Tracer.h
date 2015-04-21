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
				*color = RecursiveTrace(ray, samples, maxdepth_);
				color->Clamp();
			}

			inline void SetScene(const Scene *scene){ scene_ = scene; }

		protected:
			virtual Color RecursiveTrace(const Ray& ray, const CameraSample& samples, int depth);
			Color TraceReflection(const Ray& ray, const CameraSample& samples, const LocalGeo& geo, float reflectivity, int depth);
			Color TraceRefraction(const Ray& ray, const CameraSample& samples, const LocalGeo& geo, int depth);
			void Shade(const Ray& ray, const CameraSample& samples, const LocalGeo& geo, Color *out);

		private:
			int maxdepth_;
			const Scene *scene_;
		};
	}
}