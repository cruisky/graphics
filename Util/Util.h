#pragma once
#pragma warning(disable: 4800)
#pragma warning(disable: 4005)

namespace TX
{
	typedef unsigned int	 uint;
	typedef	signed	 __int8	 int8;
	typedef unsigned __int8	 uint8;
	typedef signed	 __int16 int16;
	typedef unsigned __int16 uint16;
	typedef	signed	 __int32 int32;
	typedef unsigned __int32 uint32;
	typedef	signed	 __int64 int64;
	typedef unsigned __int64 uint64;

	// Graphics
	class BSDF;
	class Camera;
	class Color;
	class Film;
	class Filter;
	class FontMap;
	class Image;
	class Intersection; class LocalGeo;
	class Light;
	class ObjMaterial; class ObjMesh; class ObjShape;
	class Primitive;
	class PrimitiveManager;
	class Scene;
	class SceneObject;
	class Shape;
	class Tracer;
	class Transform;

	// Math
	struct Rect; class Polygon;
	class Matrix3x3; class Matrix4x4;
	class RNG;
	class Ray;
	class Sample; class CameraSample;
	class Sampler;

	// SSE
	namespace SSE{
		struct V4Bool;
		struct V4Float;
		struct V4Int;
	}

	// System
	namespace UI{
		class Application;
		struct Input;
	}
	class Timer;
}
