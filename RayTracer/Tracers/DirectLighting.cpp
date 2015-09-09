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

				for (auto light = scene->lights.begin(); light < scene->lights.end(); light++){
					color += EstimateDirect(scene, ray, geom, light->get(), &Sample(), &Sample());
				}

				if (depth >= 0){
					color += TraceSpecularReflect(scene, ray, geom, depth - 1, samplebuf);
					color += TraceSpecularTransmit(scene, ray, geom, depth - 1, samplebuf);
				}
			}
			else {
				// TODO environment map
				color = Color(ray.dir.x, ray.dir.y, ray.dir.z) * 0.5f + 0.5f;
			}
			return color;
		}

		void DirectLighting::BakeSamples(const Scene *scene, const CameraSample *samplebuf){
			return;
		}
	}
}
