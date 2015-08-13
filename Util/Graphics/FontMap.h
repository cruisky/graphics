#pragma once

#include <GL/glew.h>
#include "Math/Geometry.h"

namespace TX
{
	class FontMap
	{
	public:
		FontMap();
		~FontMap();
		void Load(const char* file, float fontHeight=12.f);
		bool GetChar(const char *ch, Vector2& pos, Rect *rect, Rect *uv = nullptr) const;
		float GetWidth(char c) const;
		float GetWidth(const char *str) const;
		inline GLuint TexID() const { return texID; }
		inline float Height() const { return fontHeight; }
	private:
		static const int bitmapSize = 512;	// no guarantee this fits
		GLuint texID;
		float fontHeight;
		void *data;
	};
}