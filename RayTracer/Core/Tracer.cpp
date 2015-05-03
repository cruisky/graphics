#include "stdafx.h"

#include "Tracer.h"
#include "Scene.h"
#include "Intersection.h"
#include "BSDF.h"
#include "Sample.h"
#include "Color.h"
#include "Ray.h"

namespace Cruisky
{
	namespace RayTracer
	{
		//Color Tracer::TraceReflection(const Ray& ray, const LocalGeo& geo, float reflectivity, int depth){
		//	Ray reflected;
		//	reflected.dir = ray.dir + 2.f * Math::Abs(Dot(ray.dir, geo.normal)) * geo.normal;
		//	reflected.origin = geo.point + Ray::EPSILON * reflected.dir;
		//	return Li(reflected, depth) * reflectivity;
		//}

		//Color Tracer::TraceRefraction(const Ray& ray, const LocalGeo& geo, int depth){
		//	const Dielectric *diel = dynamic_cast<const Dielectric*>(geo.bsdf);
		//	if (!diel) return Color::BLACK;
		//	Ray refracted;
		//	// compute refraction
		//	const Vector3& d = ray.dir;
		//	const Vector3& n = geo.normal;
		//	float dn = Dot(d, n);
		//	float iorinv = diel->RefractiveIndexInv();
		//	float emergeAngle = 1.f - (1.f - dn * dn) * (iorinv * iorinv);
		//	// check side
		//	Color tint(1);
		//	float costheta = Math::Abs(dn);		// cos of angle between normal and viewing ray
		//	float reflectivity;
		//	if (dn > 0) {
		//		// ray goes out of the object
		//		tint = diel->Attenuation(geo.dist);
		//		if (emergeAngle < 0)
		//			return tint * TraceReflection(ray, geo, 1.f, depth);	// total internal reflection
		//	}
		//	// compute refracted ray
		//	refracted.dir = iorinv * (d + Math::Abs(dn) * n) - Math::Sqrt(emergeAngle) * n;
		//	refracted.origin = geo.point + Ray::EPSILON * refracted.dir;
		//	reflectivity = diel->Reflectivity(costheta);
		//	return tint * (TraceReflection(ray, geo, reflectivity, depth) + Li(refracted, depth) * (1 - reflectivity));
		//}

		Color Tracer::TraceDirectLight(const Ray& ray, const LocalGeo& geom, const Light *light, const Sample *lightsample, const Sample *bsdfsample){
			Vector3 wo = -ray.dir;		// dir to camera
			Ray lightray;
			float light_pdf, bsdf_pdf;
			Color color, lightcolor, surfacecolor;

			// sample light sources
			light->Illuminate(geom.point, lightsample, &lightray, &lightcolor, &light_pdf);
			if (light_pdf > 0.f && lightcolor != Color::BLACK){
				surfacecolor = geom.bsdf->Eval(lightray.dir, wo, geom);
				if (surfacecolor != Color::BLACK && !scene_->Occlude(lightray)){
					color = surfacecolor * lightcolor * Math::Abs(Dot(lightray.dir, geom.normal)) / light_pdf;
					if (light->IsDelta())
						return color;
					// multiple importance sampling
					bsdf_pdf = geom.bsdf->Pdf(wo, lightray.dir, geom);
					color *= PowerHeuristic(1, light_pdf, 1, bsdf_pdf);
				}
			}

			if (light->IsDelta()) return color;

			// sample bsdf
			BSDFType sampled;
			surfacecolor = geom.bsdf->Scatter(wo, geom, *bsdfsample, &lightray.dir, &bsdf_pdf, BSDFType(BSDF_ALL & ~BSDF_SPECULAR), &sampled);
			if (bsdf_pdf > 0.f && surfacecolor != Color::BLACK){
				light_pdf = light->Pdf(geom.point, lightray.dir);
				if (light_pdf > 0.f){
					// TODO
					//LocalGeo intxn;
					//if (scene_->Intersect(lightray, intxn)){
						// Arealight
					//} 
					//else if environmentmap == light
					if (lightcolor != Color::BLACK){
						float misWeight = PowerHeuristic(1, light_pdf, 1, bsdf_pdf);
						color += surfacecolor * lightcolor * Math::Abs(Dot(lightray.dir, geom.normal)) * misWeight;
					}
				}
			}
			return color;
		}

		Color Tracer::TraceSpecularReflect(const Ray& ray, const LocalGeo& geom, int depth){
			Vector3 wo = -ray.dir, wi;
			float pdf;
			Color color;
			Color f = geom.bsdf->Scatter(wo, geom, Sample(), &wi, &pdf, BSDFType(BSDF_REFLECTION | BSDF_SPECULAR));
			float absdot_wi_n = Math::Abs(Dot(wi, geom.normal));
			if (pdf > 0.f && f != Color::BLACK && absdot_wi_n != 0.f){
				Ray reflected(geom.point, wi);
				// TODO differential
				color = f * Li(reflected, depth) *absdot_wi_n / pdf;
			}
			return color;
		}

		Color Tracer::TraceSpecularTransmit(const Ray& ray, const LocalGeo& geom, int depth){
			Vector3 wo = -ray.dir, wi;
			float pdf;
			Color color;
			Color f = geom.bsdf->Scatter(wo, geom, Sample(), &wi, &pdf, BSDFType(BSDF_TRANSMISSION | BSDF_SPECULAR));
			float absdot_wi_n = Math::Abs(Dot(wi, geom.normal));
			if (pdf > 0.f && f != Color::BLACK && absdot_wi_n != 0.f){
				Ray refracted(geom.point, wi);
				//TODO differential
				color = f * Li(refracted, depth) * absdot_wi_n / pdf;
			}
			return color;
		}
	}
}