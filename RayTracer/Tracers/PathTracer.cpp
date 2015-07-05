#include "stdafx.h"
#include "PathTracer.h"
#include "Core/Intersection.h"
#include "Core/Scene.h"
#include "Core/Sample.h"
#include "Core/BSDF.h"
#include "Math/RNG.h"

namespace TX{
	namespace RayTracer{
		const int PathTracer::SAMPLE_DEPTH = 3;

		PathTracer::PathTracer(const Scene *scene, int maxdepth) : Tracer(scene, maxdepth){
			light_samples_.resize(maxdepth);
			bsdf_samples_.resize(maxdepth);
			scatter_samples_.resize(maxdepth);
		}

		Color PathTracer::Li(const Ray& ray, int ignoreddepth, const CameraSample& samplebuf){
			Color Le, L, pathThroughput;
			Vector3 wo, wi;
			float pdf;
			BSDFType sampled;
			Ray pathRay;
			LocalGeo geom;

			Le = L = Color::BLACK;
			pathThroughput = Color::WHITE;

			auto countLights = scene_->lights.size();
			bool specBounce = true;

			pathRay = ray;
			const Sample *lightsample, *bsdfsample, *scattersample;

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
						lightsample = light_samples_[bounce](samplebuf);
						bsdfsample = bsdf_samples_[bounce](samplebuf);
						int lightIdx = (int)Math::Min(lightsample->w * countLights, countLights - 1);
						L += pathThroughput * TraceDirectLight(pathRay, geom, scene_->lights[lightIdx].get(), lightsample, bsdfsample);
					}
					scattersample = scatter_samples_[bounce](samplebuf);
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

		void PathTracer::BakeSamples(const Scene *scene, const CameraSample *samplebuf){
			for (auto i = 0; i < maxdepth_; ++i)
			{
				light_samples_[i].RequestSamples(1, samplebuf);
				bsdf_samples_[i].RequestSamples(1, samplebuf);
				scatter_samples_[i].RequestSamples(1, samplebuf);
			}
		}
	}
}
