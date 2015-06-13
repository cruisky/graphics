#include "stdafx.h"
#include "PathTracer.h"
#include "Core/Intersection.h"
#include "Core/Scene.h"
#include "Core/Sample.h"
#include "Core/BSDF.h"
#include "RNG.h"

namespace Cruisky{
	namespace RayTracer{
		const int PathTracer::SAMPLE_DEPTH = 3;

		Color PathTracer::Li(const Ray& ray, int ignoreddepth){
			static Color Le;
			static Color L;
			static Color pathThroughput;
			static Vector3 wo;
			static Vector3 wi;
			static float pdf;
			static BSDFType sampled;

			Le = L = Color::BLACK;
			pathThroughput = Color::WHITE;

			auto countLights = scene_->lights.size();
			bool specBounce = true;
			Ray pathRay = ray;
			LocalGeo geom;
			Sample *lightsample, *bsdfsample, *scattersample;

			for (int bounce = 0; bounce < maxdepth_; ++bounce){
				if (scene_->Intersect(pathRay, geom)){
					scene_->PostIntersect(pathRay, geom);
					geom.ComputeDifferentials(pathRay);

					wo = -pathRay.dir;

					if (bounce == 0 || specBounce){
						geom.Emit(-pathRay.dir, &Le);
						L += Le * pathThroughput;
					}

					if (!geom.bsdf->IsSpecular()){
						lightsample = light_samples_[bounce];
						bsdfsample = bsdf_samples_[bounce];
						int lightIdx = Math::Min(lightsample->w * countLights, countLights - 1);
						L += pathThroughput * TraceDirectLight(pathRay, geom, scene_->lights[lightIdx].get(), lightsample, bsdfsample);
					}
					scattersample = scatter_samples_[bounce];
					Color f = geom.bsdf->Scatter(wo, geom, *scattersample, &wi, &pdf, BSDF_ALL, &sampled);
					if (f == Color::BLACK || pdf == 0.f)
						break;
					specBounce = (sampled & BSDF_SPECULAR) != 0;
					pathThroughput *= f * (Math::Abs(Dot(wi, geom.normal)) / pdf);
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
					}
					break;
				}
			}
			return L;
		}

		void PathTracer::ProcessSamples(){
			light_samples_.resize(maxdepth_);
			bsdf_samples_.resize(maxdepth_);
			scatter_samples_.resize(maxdepth_);

			for (auto i = 0; i < maxdepth_; ++i)
			{
				light_samples_[i] = samples_->RequestSamples(1);
				bsdf_samples_[i] = samples_->RequestSamples(1);
				scatter_samples_[i] = samples_->RequestSamples(1);
			}
		}
	}
}