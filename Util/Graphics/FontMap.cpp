#include "UtilStdAfx.h"
#include "FontMap.h"
#include "System/Memory.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include "Libs/stb_truetype.h"

namespace TX
{
	float GlyphPosMap::GetWidth(int index, int count) const {
		int max = map.size() - 1;
		if (max <= 0) return 0.f;
		int begin = Math::Clamp(index, 0, max);
		int end = Math::Clamp(index + count, 0, max);
		return begin == end ? 0.f : map[end] - map[begin];
	}

	int GlyphPosMap::GetIndex(float offset) const {
		auto ceiling = std::lower_bound(map.begin(), map.end(), offset);
		if (ceiling != map.begin() && ceiling != map.end()){
			return (*ceiling - offset) < (offset - *(ceiling - 1)) ? 
				(ceiling - map.begin()) : 
				(ceiling - 1 - map.begin());
		}
		else {
			return Math::Clamp(
				int(ceiling - map.begin()), 
				0, 
				int(map.size() - 1));
		}
	}

	void GlyphPosMap::Recalculate(const FontMap *font, const char *text){
		Clear();
		Vector2 pos;
		int length = std::strlen(text);
		map.reserve(length + 1);

		while (*text){
			font->GetChar(text++, pos, nullptr, nullptr, this);
		}
	}


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
			char *buf = new char[static_cast<size_t>(size)];
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
	bool FontMap::GetChar(const char *ch, Vector2& pos, Rect *rect, Rect *uv, GlyphPosMap *posMap) const {
		if (*ch >= 32 && *ch < 128){
			float left = pos.x;
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(reinterpret_cast<stbtt_bakedchar*>(data),
				bitmapSize, bitmapSize,
				*ch - 32, &pos.x, &pos.y, &q, 1);
			if (posMap)
				posMap->Append(pos.x - left);
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