#pragma once
#include "fwddecl.h"

#include "Color.h"

namespace Cruisky{
	namespace RayTracer{
		class Tracer {
		public:
			Tracer(int maxdepth = 5) : maxdepth_(maxdepth){}
			virtual ~Tracer(){}

			inline void Trace(const Ray& ray, Color *color){
				*color = RecursiveTrace(ray, maxdepth_);
				color->Clamp();
			}

			inline void SetScene(const Scene *scene){ scene_ = scene; }

		protected:
			virtual Color RecursiveTrace(const Ray& ray, int depth);
			inline bool Reflect(const Ray& ray, const LocalGeo& geo, Ray *reflected);
			inline bool Refract(const Ray& ray, const LocalGeo& geo, Ray *refracted);
			inline Color TraceReflection(const Ray& reflected, const LocalGeo& geo, int depth);
			inline Color TraceRefraction(const Ray& refracted, const LocalGeo& geo, int depth);
			inline void Shade(const Ray& ray, const LocalGeo& geo, Color *out);

		private:
			int maxdepth_;
			const Scene *scene_;
		};
	}
}