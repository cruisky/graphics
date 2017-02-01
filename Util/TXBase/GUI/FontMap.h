#pragma once

#include "TXBase/Math/Geometry.h"

namespace TX {
	class GlyphPosMap;
	class FontMap;

	class GlyphPosMap {
	public:
		GlyphPosMap() { Clear(); }
		~GlyphPosMap() {}
		inline int Size() { return map.size(); }
		inline void Clear() { map.clear(); sum = 0.f; Append(0.f); }
		inline void Append(float width) { map.push_back(sum += width); }
		float GetWidth(int index, int count) const;
		int GetIndex(float offset) const;
		void Recalculate(const FontMap *font, const std::string& text);
	private:
		std::vector<float> map;
		float sum;
	};

	class FontMap {
	public:
		FontMap();
		~FontMap();
		void LoadFile(const std::string& file, float fontHeight = 12.f);
		void Load(const unsigned char *buf, float height);
		void LoadDefault();

		bool GetChar(const char& ch, Vec2& pos, Rect *rect, Rect *uv = nullptr, GlyphPosMap *posMap = nullptr) const;
		float GetWidth(char c) const;
		float GetWidth(const std::string& str) const;
		inline uint TexID() const { return texID_; }
		inline float Height() const { return fontHeight_; }
	private:
		static const int bitmapSize = 512;	// no guarantee this fits
		uint texID_;
		float fontHeight_;
		void *data_;
	};
}
