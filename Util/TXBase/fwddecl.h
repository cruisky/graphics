#pragma once
#pragma warning(disable: 4800)
#pragma warning(disable: 4005)

#include <cassert>
#include <memory>
#include <vector>
#include <cstring>
#include <iostream>

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

	class Filter;
	class Image;
	class Film;

	class Camera;
	class Shape; class Mesh;
	class ObjMaterial; class ObjMesh; class ObjShape;
	class FontMap;

	// Math
	class Color;
	struct Rect; class Polygon;
	class Matrix3x3; class Matrix4x4;
	class Transform;
	class RNG;
	class Ray;
	class Sample; class CameraSample; class Distribution1D;

	template<size_t N, typename T> class Vec;
	typedef Vec<2, float> Vec2;
	typedef Vec<2, int> Vec2i;
	typedef Vec<2, uint> Vec2u;
	typedef Vec<3, float> Vec3;
	typedef Vec<3, int> Vec3i;
	typedef Vec<3, uint> Vec3u;
	typedef Vec<4, float> Vec4;
	typedef Vec<4, int> Vec4i;
	typedef Vec<4, uint> Vec4u;

	// SSE
	namespace SSE{
		struct V4Bool;
		struct V4Float;
		struct V4Int;
	}

	// GUI
	namespace UI{
		class Application;
		struct Input;
	}
	class Timer;
}
