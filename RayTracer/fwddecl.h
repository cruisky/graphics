#pragma once

namespace Cruisky{
	
	class Ray;
	class Color;
	class Vector3;
	class Matrix4x4;

	namespace RayTracer
	{
		class BSDF;
		struct Intersection; struct LocalGeo;
		class Light;
		class Primitive;
		class PrimitiveManager;
		class Scene;
		class Shape;
		class Transform;
	}
}