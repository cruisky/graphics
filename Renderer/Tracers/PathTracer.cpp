#include "stdafx.h"
#include "PathTracer.h"
#include "Math/Sample.h"
#include "Math/Random.h"
#include "Graphics/Intersection.h"
#include "Graphics/Scene.h"
#include "Graphics/BSDF.h"

namespace TX{
	const int PathTracer::SAMPLE_DEPTH = 3;

	PathTracer::PathTracer(int maxdepth) : Tracer(maxdepth){
		light_samples_.resize(maxdepth);
		bsdf_samples_.resize(maxdepth);
		scatter_samples_.resize(maxdepth);
	}

	Color PathTracer::Li(const Scene *scene, const Ray& ray, int ignoreddepth, const CameraSample& samplebuf){
		Color Le, L, pathThroughput = Color::WHITE;
		Vec3 wo, wi;
		float pdf;
		BSDFType sampled;
		Ray pathRay;
		LocalGeo geom;

		auto countLights = scene->lights.size();
		bool specBounce = true;

		pathRay = ray;
		const Sample *lightsample, *bsdfsample, *scattersample;

		for (int bounce = 0; bounce < maxdepth_; ++bounce){
			if (scene->Intersect(pathRay, geom)){
				scene->PostIntersect(pathRay, geom);
				geom.ComputeDifferentials(pathRay);

				wo = -pathRay.dir;

				// Emit radiance if the intersection is emitter
				if (specBounce){
					geom.Emit(-pathRay.dir, &Le);
					L += pathThroughput * Le;
				}

				if (!geom.bsdf->IsSpecular()){
					lightsample = light_samples_[bounce](samplebuf);
					bsdfsample = bsdf_samples_[bounce](samplebuf);
					int lightIdx = (int)Math::Min(lightsample->w * countLights, countLights - 1);
					L += pathThroughput * EstimateDirect(scene, pathRay, geom, scene->lights[lightIdx].get(), lightsample, bsdfsample);
				}
				scattersample = scatter_samples_[bounce](samplebuf);
				Color f = geom.bsdf->SampleDirect(wo, geom, *scattersample, &wi, &pdf, BSDF_ALL, &sampled);
				if (f == Color::BLACK || pdf == 0.f)
					break;
				specBounce = (sampled & BSDF_SPECULAR) != 0;
				pathThroughput *= f * (Math::AbsDot(wi, geom.normal) / pdf);
				pathRay = Ray(geom.point, wi);

				// Russian Roulette
				if (bounce > SAMPLE_DEPTH){
					float probContinue = Math::Min(1.f, pathThroughput.Luminance());
					if (rng_->Float() > probContinue)
						break;
					pathThroughput /= probContinue;
				}
			}
			else {
				if (specBounce){
					// TODO environment light
					L += pathThroughput * (Color(ray.dir.x, ray.dir.y, ray.dir.z) * 0.5f + 0.5f);
				}
				break;
			}
		}
		return L;
	}

	void PathTracer::BakeSamples(const Scene *scene, const CameraSample *samplebuf){
		for (auto i = 0; i < maxdepth_; ++i)
		{
			light_samples_[i].RequestSamples(1, samplebuf);
			bsdf_samples_[i].RequestSamples(1, samplebuf);
			scatter_samples_[i].RequestSamples(1, samplebuf);
		}
	}
}
