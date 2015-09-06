#pragma once

namespace TX{
	class Color;
	class Filter;
	class Ray;

	class Vector3;
	class Vector4;
	class Matrix4x4;

	class RNG;
	class Timer;
	class IProgressMonitor;
	class ProgressMonitor;

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
