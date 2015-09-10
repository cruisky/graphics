#include "UtilStdAfx.h"
#include "Graphics/Color.h"

namespace TX{
	const Color Color::BLACK = Color(0.f);
	const Color Color::WHITE = Color(1.f);
	const Color Color::RED = Color(1.f, 0.f, 0.f);
	const Color Color::GREEN = Color(0.f, 1.f, 0.f);
	const Color Color::BLUE = Color(0.f, 0.f, 1.f);

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