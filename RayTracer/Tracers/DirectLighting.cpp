#include "stdafx.h"
#include "DirectLighting.h"
#include "Core/Intersection.h"
#include "Core/BSDF.h"
#include "Core/Scene.h"
#include "Core/Sample.h"

namespace TX{
	namespace RayTracer {
		DirectLighting::DirectLighting(int maxdepth) : Tracer(maxdepth){}

		Color DirectLighting::Li(const Scene *scene, const Ray& ray, int depth, const CameraSample& samplebuf){
			if (depth < 0)
				return Color::BLACK;
			LocalGeo geom;
			Color color;
			if (scene->Intersect(ray, geom)){
				scene->PostIntersect(ray, geom);
				geom.ComputeDifferentials(ray);

				for (int i = 0; i < light_samples_.size(); i++){
					color += TraceDirectLight(scene, ray, geom, scene->lights[i].get(), light_samples_[i](samplebuf), bsdf_samples_[i](samplebuf)) * Math::PI;
				}

				if (depth >= 0){
					color += TraceSpecularReflect(scene, ray, geom, depth + 1, samplebuf);
					color += TraceSpecularTransmit(scene, ray, geom, depth + 1, samplebuf);
				}
			}
			else {
				// TODO environment map
			}
			return color;
		}

		void DirectLighting::BakeSamples(const Scene *scene, const CameraSample *samplebuf){
			light_samples_.resize(scene->lights.size());
			bsdf_samples_.resize(scene->lights.size());
			auto lights = scene->lights;
			for (auto i = 0; i < scene->lights.size(); i++){
				// need equal amount of samples for light and bsdf sampling
				// for multiple importance sampling
				light_samples_[i].RequestSamples(lights[i]->sample_count, samplebuf);
				bsdf_samples_[i].RequestSamples(lights[i]->sample_count, samplebuf);
			}
		}
	}
}
