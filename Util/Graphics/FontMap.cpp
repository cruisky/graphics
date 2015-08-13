#include "FontMap.h"

#include <fstream>
#include "System/Memory.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include "Libs/stb_truetype.h"

namespace TX
{
	FontMap::FontMap() {
		// initialize font texture
		glGenTextures(1, &texID);
		data = new stbtt_bakedchar[96];	// ASCII 32..126;
	}
	FontMap::~FontMap(){
		glDeleteTextures(1, &texID);
		MemDeleteArray(data);
	}

	void FontMap::Load(const char *file, float height){
		fontHeight = height;
		std::ifstream in(file, std::ifstream::ate | std::ifstream::binary);
		if (in.is_open()){
			std::streampos size = in.tellg();
			char *buf = new char[size];
			AutoDeleteArray<char> _(buf);
			in.seekg(0, std::ios::beg);
			in.read(buf, size);
			in.close();

			unsigned char tempBitmap[bitmapSize * bitmapSize];
			int res = stbtt_BakeFontBitmap(reinterpret_cast<unsigned char *>(buf), 0, height, tempBitmap, bitmapSize, bitmapSize, 32, 96, reinterpret_cast<stbtt_bakedchar*>(data));
			if (res > 0){
				glBindTexture(GL_TEXTURE_2D, texID);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, bitmapSize, bitmapSize, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tempBitmap);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else {
				throw "failed to load font, ";
			}
		}
		else{
			throw "failed to open file: " + std::string(file);
		}
	}
	bool FontMap::GetChar(const char *ch, Vector2& pos, Rect *rect, Rect *uv) const {
		if (*ch >= 32 && *ch < 128){
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(reinterpret_cast<stbtt_bakedchar*>(data),
				bitmapSize, bitmapSize,
				*ch - 32, &pos.x, &pos.y, &q, 1);
			if (uv){
				uv->min.x = q.s0; uv->min.y = q.t0;
				uv->max.x = q.s1; uv->max.y = q.t1;
			}
			if (rect){
				rect->min.x = q.x0; rect->min.y = q.y0;
				rect->max.x = q.x1; rect->max.y = q.y1;
			}
			return true;
		}
		else {
			return false;
		}
	}
	float FontMap::GetWidth(char c) const {
		Rect r; Vector2 pos;
		if (GetChar(&c, pos, &r)){
			return pos.x;
		}
		else {
			return 0.f;
		}
	}
	float FontMap::GetWidth(const char *str) const {
		Rect r;
		Vector2 pos;
		while (*str && GetChar(str++, pos, &r));
		return pos.x;
	}
}