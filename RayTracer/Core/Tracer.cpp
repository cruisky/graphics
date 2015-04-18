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
			Color color;
			const Mirror *mirror;
			if (scene_->Intersect(ray, geo)){
				scene_->PostIntersect(geo);
				Shade(ray, geo, &color);

				mirror = dynamic_cast<const Mirror *>(geo.bsdf);
				if (mirror)
					color += TraceReflection(ray, geo, mirror->reflectivity, depth - 1);
				//if (Refract(ray, geo, &refracted))
				//	color += TraceRefraction(refracted, geo, depth - 1);
			}
			return color;
		}

		Color Tracer::TraceReflection(const Ray& ray, const LocalGeo& geo, float reflectivity, int depth){
			Ray reflected;
			reflected.dir = ray.dir + 2.f * Math::Abs(Dot(ray.dir, geo.normal)) * geo.normal;
			reflected.origin = geo.point + Ray::EPSILON * reflected.dir;
			return RecursiveTrace(reflected, depth) * reflectivity;
		}

		Color Tracer::TraceRefraction(const Ray& ray, const LocalGeo& geo, int depth){
			const Dielectric *diel = dynamic_cast<const Dielectric*>(geo.bsdf);
			if (!diel) return Color::BLACK;
			Ray refracted;
			// compute refraction
			const Vector3& d = ray.dir;
			const Vector3& n = geo.normal;
			float dn = Dot(d, n);
			float iorinv = diel->RefractiveIndexInv();
			assert(iorinv != BSDF::NOT_REFRACTIVE);
			float emergeAngle = 1.f - (1.f - dn * dn) * (iorinv * iorinv);
			// check side
			Color tint(1);
			float costheta = Math::Abs(dn);		// cos of angle between normal and viewing ray
			float reflectivity;
			if (dn > 0) {
				// ray goes out of the object
				tint = diel->Attenuation(geo.dist);
				if (emergeAngle < 0)
					return tint * TraceReflection(ray, geo, 1.f, depth);	// total internal reflection
			}
			// compute refracted ray
			refracted.dir = iorinv * (d + Math::Abs(dn) * n) - Math::Sqrt(emergeAngle) * n;
			refracted.origin = geo.point + Ray::EPSILON * refracted.dir;
			reflectivity = diel->Reflectivity(costheta);
			return tint * (TraceReflection(ray, geo, reflectivity, depth) + RecursiveTrace(refracted, depth) * (1 - reflectivity));
		}

		void Tracer::Shade(const Ray& ray, const LocalGeo& geo, Color *out){
			const int lightcount = scene_->lights.size();
			Color lightcolor;
			Ray lightray;
			Vector3 wo = -ray.dir;		// dir to camera
			for (int i = 0; i < lightcount; i++){
				scene_->lights[i]->Emit(geo, &lightray, &lightcolor);
				if (!scene_->Occlude(lightray)){
					*out += geo.bsdf->Eval(lightray.dir, wo, geo) * lightcolor;
				}
			}
		}
	}
}