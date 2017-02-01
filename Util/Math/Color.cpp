#include "UtilStdAfx.h"
#include "Math/Color.h"

namespace TX{
	const Color Color::ZERO = Color(0, 0, 0, 0);
	const Color Color::BLACK = Color(0);
	const Color Color::WHITE = Color(1);
	const Color Color::RED = Color(1, 0, 0);
	const Color Color::GREEN = Color(0, 1, 0);
	const Color Color::BLUE = Color(0, 0, 1);

	Color Color::RGBA(uint32 rgba){
		return Color((rgba >> 24) / 255.f,
			((rgba >> 16) & 0xFF) / 255.f,
			((rgba >> 8) & 0xFF) / 255.f,
			(rgba & 0xFF) / 255.f);
	}
	Color Color::RGB(uint32 rgb){
		return RGBA((rgb << 8) | 0xFF);
	}
}
