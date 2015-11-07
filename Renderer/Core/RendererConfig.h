#pragma once


#include "Graphics/Filter.h"
#include "Graphics/Tracer.h"
#include "Math/Sampler.h"
#include "Math/Sample.h"

#include "Tracers/DirectLighting.h"
#include "Tracers/PathTracer.h"
#include "Samplers/RandomSampler.h"

namespace TX
{
	enum class TracerType{
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
		TracerType tracer_t = TracerType::PathTracing;
		int tracer_maxdepth = 5;
		SamplerType sampler_t = SamplerType::Random;

		Tracer* NewTracer() const {
			switch (tracer_t){
			case TracerType::DirectLighting:
				return new DirectLighting(tracer_maxdepth);
			case TracerType::PathTracing:
				return new PathTracer(tracer_maxdepth);
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
