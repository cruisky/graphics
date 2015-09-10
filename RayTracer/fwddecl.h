#pragma once

#include "Util.h"

namespace TX{
	namespace RayTracer
	{
		class BSDF;
		class Camera;
		enum class TracerType; enum class SamplerType;
		class Film;
		struct Intersection; struct LocalGeo;
		class Light; class VisibilityTester;
		class Primitive;
		class PrimitiveManager;
		class Renderer; struct RendererConfig;
		struct Sample; class CameraSample; class SampleOffset;
		class Sampler;
		class Scene;
		class SceneObject;
		class Shape;
		class Synchronizer; class RenderTask; struct RenderTile;
		class Tracer;
		class Transform;
	}
}
