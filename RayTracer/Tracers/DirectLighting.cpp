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
			LocalGeo geo;
			Color color;
			const Mirror *mirror;
			if (scene_->Intersect(ray, geo)){
				scene_->PostIntersect(geo);

				auto lightcount = scene_->lights.size();
				for (auto i = 0; i < lightcount; i++){
					color += TraceDirectLight(ray, geo, scene_->lights[i].get(), light_samples_[i], bsdf_samples_[i]);
				}
					mirror = dynamic_cast<const Mirror *>(geo.bsdf);
					if (mirror)
						color += TraceReflection(ray, geo, mirror->reflectivity, depth - 1);
					color += TraceRefraction(ray, geo, depth - 1);
			}
			return color;
		}

		void DirectLighting::ProcessSamples(){
			auto lights = scene_->lights;
			light_samples_.resize(lights.size());
			bsdf_samples_.resize(lights.size());
			int offset = 0, tail = 0;
			for (auto i = 0; i < lights.size(); i++){
				light_samples_[i] = samples_->RequestSamples(lights[i]->sample_count);
				bsdf_samples_[i] = samples_->RequestSamples(lights[i]->sample_count);
			}
		}
	}
}