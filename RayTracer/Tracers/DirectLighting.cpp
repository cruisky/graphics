#include "stdafx.h"
#include "DirectLighting.h"
#include "Core/Intersection.h"
#include "Core/BSDF.h"
#include "Core/Scene.h"
#include "Core/Sample.h"

namespace TX{
	namespace RayTracer {
		DirectLighting::DirectLighting(const Scene *scene, int maxdepth) : Tracer(scene, maxdepth){
			auto lights = scene_->lights;
			auto size = lights.size();
			printf("size=%d\n", size);
			light_samples_.resize(lights.size());
			bsdf_samples_.resize(lights.size());
		}

		Color DirectLighting::Li(const Ray& ray, int depth, const CameraSample& samplebuf){
			if (depth < 0)
				return Color::BLACK;
			LocalGeo geom;
			Color color;
			if (scene_->Intersect(ray, geom)){
				scene_->PostIntersect(ray, geom);
				geom.ComputeDifferentials(ray);

				int lightcount = scene_->lights.size();
				for (int i = 0; i < lightcount; i++){
					color += TraceDirectLight(ray, geom, scene_->lights[i].get(), light_samples_[i](samplebuf), bsdf_samples_[i](samplebuf)) * Math::PI;
				}

				if (depth >= 0){
					color += TraceSpecularReflect(ray, geom, depth + 1, samplebuf);
					color += TraceSpecularTransmit(ray, geom, depth + 1, samplebuf);
				}
			}
			else {
				// TODO environment map
			}
			return color;
		}

		void DirectLighting::BakeSamples(const Scene *scene, const CameraSample *samplebuf){
			auto lights = scene->lights;
			for (auto i = 0; i < scene_->lights.size(); i++){
				// need equal amount of samples for light and bsdf sampling
				// for multiple importance sampling
				light_samples_[i].RequestSamples(lights[i]->sample_count, samplebuf);
				bsdf_samples_[i].RequestSamples(lights[i]->sample_count, samplebuf);
			}
		}
	}
}
