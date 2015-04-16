#include "stdafx.h"

#include "Tracer.h"
#include "Scene.h"
#include "Intersection.h"
#include "BSDF.h"
#include "Color.h"
#include "Ray.h"

namespace Cruisky
{
	namespace RayTracer
	{
		Color Tracer::RecursiveTrace(const Ray& ray, int depth){
			if (depth < 0)
				return Color::BLACK;
			LocalGeo geo;
			//Ray reflected, refracted;
			Color color;
			if (scene_->Intersect(ray, geo)){
				scene_->PostIntersect(geo);
				Shade(ray, geo, &color);

				//if (Reflect(ray, geo, &reflected))
				//	color += TraceReflection(reflected, geo, depth - 1);
				//if (Refract(ray, geo, &refracted))
				//	color += TraceRefraction(refracted, geo, depth - 1);
			}
			return color;
		}

		bool Tracer::Reflect(const Ray& ray, const LocalGeo& geo, Ray *reflected){
			reflected->dir = ray.dir + 2.f * Math::Abs(Dot(ray.dir, geo.normal)) * geo.normal;
			reflected->origin = geo.point + Ray::EPSILON * reflected->dir;
			return true;
		}

		bool Tracer::Refract(const Ray& ray, const LocalGeo& geo, Ray *refracted){
			const Vector3& d = ray.dir;
			const Vector3& n = geo.normal;
			float dn = Dot(d, n);
			float iorinv = geo.bsdf->RefractiveIndexInv();
			assert(iorinv != BSDF::NOT_REFRACTIVE);
			float emergeAngle = 1.f - (1.f - dn * dn) * (iorinv * iorinv);
			if (emergeAngle < 0)
				return false;
			refracted->dir = iorinv * (d + Math::Abs(dn) * n) - Math::Sqrt(emergeAngle) * n;
			refracted->origin = geo.point + Ray::EPSILON * refracted->dir;
			return true;
		}

		Color Tracer::TraceReflection(const Ray& reflected, const LocalGeo& geo, int depth){
			//TODO
		}

		Color Tracer::TraceRefraction(const Ray& reflected, const LocalGeo& geo, int depth){
			//TODO
		}

		void Tracer::Shade(const Ray& ray, const LocalGeo& geo, Color *out){
			Color lightcolor;
			Ray lightray;
			const int lightcount = scene_->lights.size();
			const Vector3& wo = -ray.dir;		// dir to camera
			for (int i = 0; i < lightcount; i++){
				scene_->lights[i]->Emit(geo, &lightray, &lightcolor);
				if (!scene_->Occlude(lightray)){
					*out += geo.bsdf->Eval(lightray.dir, wo, geo) * lightcolor;
				}
			}
		}
	}
}