#include "UtilStdAfx.h"
#include "RayTracer.h"
#include "Scene.h"
#include "Intersection.h"
#include "BSDF.h"
#include "Math/Sample.h"

namespace TX
{
	void RayTracer::Trace(const Scene *scene, const Ray& ray, const CameraSample& samples, RNG& rng, Color *color)
	{
		rng_ = &rng;
		*color = Li(scene, ray, maxdepth_, samples);
	}

	Color RayTracer::EstimateDirect(const Scene *scene, const Ray& ray, const LocalGeo& geom, const Light *light, const Sample *lightsample, const Sample *bsdfsample){
		Vec3 wo = -ray.dir;		// dir to camera
		Ray lightray;
		float light_pdf, bsdf_pdf;
		Color color, lightcolor, surfacecolor;

		// sample light sources with multiple importance sampling
		light->SampleDirect(geom.point, lightsample, &lightray, &lightcolor, &light_pdf);
		if (light_pdf > 0.f && lightcolor != Color::BLACK){
			surfacecolor = geom.bsdf->Eval(lightray.dir, wo, geom, BSDFType(BSDF_ALL & ~BSDF_SPECULAR));
			if (surfacecolor != Color::BLACK && !scene->Occlude(lightray)){
				if (light->IsDelta())
					color = surfacecolor * lightcolor * (Math::AbsDot(lightray.dir, geom.normal));
				else{
					bsdf_pdf = geom.bsdf->Pdf(wo, lightray.dir, geom);
					color = surfacecolor * lightcolor * (Math::AbsDot(lightray.dir, geom.normal) / light_pdf * PowerHeuristic(1, light_pdf, 1, bsdf_pdf));
				}
			}
		}

		if (light->IsDelta())
			return color;

		// sample bsdf with multiple importance sampling
		surfacecolor = geom.bsdf->SampleDirect(wo, geom, *bsdfsample, &(lightray.dir), &bsdf_pdf, BSDFType(BSDF_ALL & ~BSDF_SPECULAR));
		if (bsdf_pdf > 0.f && surfacecolor != Color::BLACK){
			lightcolor = Color::BLACK;
			LocalGeo geom_light;
			if (scene->Intersect(lightray, geom_light)){
				if (light == geom_light.prim->GetAreaLight()){
					scene->PostIntersect(lightray, geom_light);
					light_pdf = geom_light.prim->Pdf(geom_light.triId, geom.point, lightray.dir);
					geom_light.Emit(-lightray.dir, &lightcolor);
				}
			}
			//else
			//	light->Emit(lightray, &lightcolor);
			if (lightcolor != Color::BLACK){
				color += surfacecolor * lightcolor * (Math::AbsDot(lightray.dir, geom.normal) / bsdf_pdf * PowerHeuristic(1, bsdf_pdf, 1, light_pdf));
			}
		}
		return color;
	}

	Color RayTracer::TraceSpecularReflect(const Scene *scene, const Ray& ray, const LocalGeo& geom, int depth, const CameraSample& samplebuf){
		Vec3 wo = -ray.dir, wi;
		float pdf;
		Color color;
		Color f = geom.bsdf->SampleDirect(wo, geom, Sample(), &wi, &pdf, BSDFType(BSDF_REFLECTION | BSDF_SPECULAR));
		float absdot_wi_n = Math::AbsDot(wi, geom.normal);
		if (pdf > 0.f && f != Color::BLACK && absdot_wi_n != 0.f){
			Ray reflected(geom.point, wi);
			// TODO differential
			color = f * Li(scene, reflected, depth, samplebuf) *absdot_wi_n / pdf;
		}
		return color;
	}

	Color RayTracer::TraceSpecularTransmit(const Scene *scene, const Ray& ray, const LocalGeo& geom, int depth, const CameraSample& samplebuf){
		Vec3 wo = -ray.dir, wi;
		float pdf;
		Color color;
		Color f = geom.bsdf->SampleDirect(wo, geom, Sample(), &wi, &pdf, BSDFType(BSDF_TRANSMISSION | BSDF_SPECULAR));
		float absdot_wi_n = Math::AbsDot(wi, geom.normal);
		if (pdf > 0.f && f != Color::BLACK && absdot_wi_n != 0.f){
			Ray refracted(geom.point, wi);
			//TODO differential
			color = f * Li(scene, refracted, depth, samplebuf) * absdot_wi_n / pdf;
		}
		return color;
	}
}
