#pragma once
#pragma warning(disable: 4800)

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
	class Color;
	struct DrawList; struct DrawCmd; struct DrawVert;
	class Filter;
	class FontMap; class GlyphPosMap;
	namespace UI{ namespace GUI{
		struct Style;
	}}
	class Image;
	class Ray;

	// Math
	struct Rect; class Polygon;
	class Vector2; class Vector3; class Vector4;
	class Matrix4x4;
	class RNG;

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
	//class Thread; class WorkerThread; class Lock; class LockGuard; class Condition; class Task; class ThreadScheduler;
	class Timer;
	class IProgressMonitor;
	class ProgressMonitor;
}