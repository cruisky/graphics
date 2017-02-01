#pragma once

#include "Core/RayTracer.h"
#include <vector>

namespace TX{
	class DirectLighting : public RayTracer {
	public:
		DirectLighting(int maxdepth = 5);
		~DirectLighting(){}

		void BakeSamples(const Scene *scene, const CameraSample *samplebuf);
	protected:
		Color Li(const Scene *scene, const Ray& ray, int depth, const CameraSample& samplebuf);
	};
}
