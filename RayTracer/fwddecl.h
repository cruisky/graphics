#pragma once

namespace Cruisky{
	
	class Ray;
	class Color;
	class Vector3;
	class Matrix4x4;
	class RNG;

	namespace RayTracer
	{
		class BSDF;
		class Camera;
		class Film;
		struct Intersection; struct LocalGeo;
		class Light;
		class Primitive;
		class PrimitiveManager;
		class Renderer;
		class Sampler; struct Sample;
		class Scene;
		class SceneObject;
		class Shape;
		class Tracer;
		class Transform;
	}
}