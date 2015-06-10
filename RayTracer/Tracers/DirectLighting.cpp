#include "stdafx.h"
#include "DirectLighting.h"
#include "Core/Intersection.h"
#include "Core/BSDF.h"
#include "Core/Scene.h"
#include "Core/Sample.h"

namespace Cruisky{
	namespace RayTracer {
		Color DirectLighting::Li(const Ray& ray, int depth){
			if (depth < 0)
				return Color::BLACK;
			LocalGeo geom;
			Color color;
			if (scene_->Intersect(ray, geom)){
				scene_->PostIntersect(geom);
				geom.ComputeDifferentials(ray);

				auto lightcount = scene_->lights.size();
				for (auto i = 0; i < lightcount; i++){
					color += TraceDirectLight(ray, geom, scene_->lights[i].get(), light_samples_[i], bsdf_samples_[i]) * Math::PI;
				}

				if (depth >= 0){
					color += TraceSpecularReflect(ray, geom, depth + 1);
					color += TraceSpecularTransmit(ray, geom, depth + 1);
				}
			}
			else {
				// TODO environment map
			}
			return color;
		}

		void DirectLighting::ProcessSamples(){
			auto lights = scene_->lights;
			light_samples_.resize(lights.size());
			bsdf_samples_.resize(lights.size());
			int offset = 0, tail = 0;
			for (auto i = 0; i < lights.size(); i++){
				// need equal amount of samples for light and bsdf sampling
				// for multiple importance sampling
				light_samples_[i] = samples_->RequestSamples(lights[i]->sample_count);
				bsdf_samples_[i] = samples_->RequestSamples(lights[i]->sample_count);
			}
		}
	}
}