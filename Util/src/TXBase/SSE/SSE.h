#pragma once

#include "txbase/fwddecl.h"
#include <intrin.h>
//#include <smmintrin.h>

#include "txbase/sse/bool.h"
#include "txbase/sse/int.h"
#include "txbase/sse/float.h"

namespace TX {
	namespace SSE {
		typedef Vec<2, V4Bool> Vec2V4B;
		typedef Vec<3, V4Bool> Vec3V4B;
		typedef Vec<4, V4Bool> Vec4V4B;

		typedef Vec<2, V4Int> Vec2V4I;
		typedef Vec<3, V4Int> Vec3V4I;
		typedef Vec<4, V4Int> Vec4V4I;

		typedef Vec<2, V4Float> Vec2V4F;
		typedef Vec<3, V4Float> Vec3V4F;
		typedef Vec<4, V4Float> Vec4V4F;
	}
}
