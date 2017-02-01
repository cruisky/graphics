#pragma once

#include "txbase/image/filter.h"
#include "txbase/math/sample.h"

#include "RayTracer.h"
#include "Methods/DirectLighting.h"
#include "Methods/PathTracing.h"
#include "Sampler.h"
#include "Samplers/RandomSampler.h"

namespace TX
{
	enum class RenderMethod{
		DirectLighting,
		PathTracing
	};
	enum class SamplerType{
		Random
	};

	struct RendererConfig {
		RendererConfig(){}
		int samples_per_pixel;
		int width = 0, height = 0;
		RenderMethod tracer_t = RenderMethod::PathTracing;
		int tracer_maxdepth = 5;
		SamplerType sampler_t = SamplerType::Random;

		RayTracer* NewMethod() const {
			switch (tracer_t){
			case RenderMethod::DirectLighting:
				return new DirectLighting(tracer_maxdepth);
			case RenderMethod::PathTracing:
				return new PathTracing(tracer_maxdepth);
			default:
				throw "unimplemented";
			}
		}

		Sampler* NewSampler() const {
			switch (sampler_t){
			case SamplerType::Random:
				return new RandomSampler;
			default:
				throw "unimplemented";
			}
		}
	};
}
