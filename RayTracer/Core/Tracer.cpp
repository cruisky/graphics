#include "stdafx.h"
#include "Tracer.h"
#include "Scene.h"
#include "Intersection.h"
#include "BSDF.h"
#include "Sample.h"

namespace TX
{
	namespace RayTracer
	{
		Color Tracer::TraceDirectLight(const Scene *scene, const Ray& ray, const LocalGeo& geom, const Light *light, const Sample *lightsample, const Sample *bsdfsample){
			Vector3 wo = -ray.dir;		// dir to camera
			Ray lightray;
			float light_pdf, bsdf_pdf;
			Color color, lightcolor, surfacecolor;

			// sample light sources with multiple importance sampling
			light->Illuminate(geom.point, lightsample, &lightray, &lightcolor, &light_pdf);
			if (light_pdf > 0.f && lightcolor != Color::BLACK && !scene->Occlude(lightray)){
				surfacecolor = geom.bsdf->Eval(lightray.dir, wo, geom);
				if (surfacecolor != Color::BLACK){
					if (light->IsDelta())
						color = surfacecolor * lightcolor * (Math::Abs(Dot(lightray.dir, geom.normal)) / light_pdf);
					else{
						bsdf_pdf = geom.bsdf->Pdf(wo, lightray.dir, geom);
						color = surfacecolor * lightcolor * (Math::Abs(Dot(lightray.dir, geom.normal)) / light_pdf * PowerHeuristic(1, light_pdf, 1, bsdf_pdf));
					}
				}
			}

			if (light->IsDelta())
				return color;

			// sample bsdf with multiple importance sampling
			BSDFType sampled;
			surfacecolor = geom.bsdf->Scatter(wo, geom, *bsdfsample, &(lightray.dir), &bsdf_pdf, BSDFType(BSDF_ALL & ~BSDF_SPECULAR), &sampled);
			if (bsdf_pdf > 0.f && surfacecolor != Color::BLACK){
				lightcolor = Color::BLACK;
				light_pdf = light->Pdf(geom.point, lightray.dir);
				if (light_pdf == 0.f)
					return color;
				LocalGeo geom_light;
				if (scene->Intersect(lightray, geom_light)){
					if (light == geom_light.GetAreaLight()){
						scene->PostIntersect(lightray, geom_light);
						geom_light.Emit(-lightray.dir, &lightcolor);
					}
				}
				//else
				//	light->Emit(lightray, &lightcolor);
				if (lightcolor != Color::BLACK){
					color += surfacecolor * lightcolor * Math::Abs(Dot(lightray.dir, geom.normal)) * PowerHeuristic(1, bsdf_pdf, 1, light_pdf) / bsdf_pdf;
				}
			}
			return color;
		}

		Color Tracer::TraceSpecularReflect(const Scene *scene, const Ray& ray, const LocalGeo& geom, int depth, const CameraSample& samplebuf){
			Vector3 wo = -ray.dir, wi;
			float pdf;
			Color color;
			Color f = geom.bsdf->Scatter(wo, geom, Sample(), &wi, &pdf, BSDFType(BSDF_REFLECTION | BSDF_SPECULAR));
			float absdot_wi_n = Math::Abs(Dot(wi, geom.normal));
			if (pdf > 0.f && f != Color::BLACK && absdot_wi_n != 0.f){
				Ray reflected(geom.point, wi);
				// TODO differential
				color = f * Li(scene, reflected, depth, samplebuf) *absdot_wi_n / pdf;
			}
			return color;
		}

		Color Tracer::TraceSpecularTransmit(const Scene *scene, const Ray& ray, const LocalGeo& geom, int depth, const CameraSample& samplebuf){
			Vector3 wo = -ray.dir, wi;
			float pdf;
			Color color;
			Color f = geom.bsdf->Scatter(wo, geom, Sample(), &wi, &pdf, BSDFType(BSDF_TRANSMISSION | BSDF_SPECULAR));
			float absdot_wi_n = Math::Abs(Dot(wi, geom.normal));
			if (pdf > 0.f && f != Color::BLACK && absdot_wi_n != 0.f){
				Ray refracted(geom.point, wi);
				//TODO differential
				color = f * Li(scene, refracted, depth, samplebuf) * absdot_wi_n / pdf;
			}
			return color;
		}
	}
}
