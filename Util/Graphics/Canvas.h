#pragma once

#include "Math/Vector.h"
#include "Color.h"

namespace TX {
	namespace UI {
		void DrawString(float x, float y, void *font, const char *text, const Color& color);
		void DrawRect(const Vector2& TL, const Vector2& BR, const Color& color, bool fill = true);
		void DrawTriangle(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Color& color, bool fill = true);
	}
}

