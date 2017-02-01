#include "TXBase/stdafx.h"
#include "Float.h"

namespace TX
{
	namespace SSE
	{
		const V4Float V4Float::X(1.f, 0.f, 0.f, 0.f);
		const V4Float V4Float::Y(0.f, 1.f, 0.f, 0.f);
		const V4Float V4Float::Z(0.f, 0.f, 1.f, 0.f);
		const V4Float V4Float::W(0.f, 0.f, 0.f, 1.f);
		const V4Float V4Float::UNIT[4] = {
			V4Float(1.f, 0.f, 0.f, 0.f),
			V4Float(0.f, 1.f, 0.f, 0.f),
			V4Float(0.f, 0.f, 1.f, 0.f),
			V4Float(0.f, 0.f, 0.f, 1.f)
		};
		const V4Float V4Float::ZERO(0.f);
		const V4Float V4Float::ONE(1.f);
		const V4Float V4Float::PI(Math::PI);
		const V4Float V4Float::PI_RCP(Math::PI_RCP);
		const V4Float V4Float::INF(Math::INF);
		const V4Float V4Float::EPSILON(Math::EPSILON);
	}
}
