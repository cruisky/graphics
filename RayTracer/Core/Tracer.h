#pragma once
#include "fwddecl.h"

namespace Cruisky{
	namespace RayTracer{
		class Tracer {
		public:
			Tracer(int maxdepth = 5) : maxdepth_(maxdepth){}

			inline void Trace(const Scene *scene, const Ray& ray, Color *color){
				RecursiveTrace(scene, ray, color, maxdepth_);
			}

		protected:
			virtual void RecursiveTrace(const Scene *scene, const Ray& ray, Color* color, int depth);
			void Reflect(const Ray& ray, const LocalGeo& geo, Ray& reflected);
			void Refract(const Ray& ray, const LocalGeo& geo, Ray& refracted);
		
		private:
			int maxdepth_;
		};
	}
}