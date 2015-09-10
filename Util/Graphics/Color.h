#pragma once

#include <iostream>
#include "Util.h"
#include "Math/MathUtil.h"

#ifdef RGB
	#undef RGB
#endif

namespace TX{
	class Color {
	public:
		enum class Channel{
			Y = 1, YA, RGB, RGBA
		};
	public:
		static const Color BLACK;
		static const Color WHITE;
		static const Color RED;
		static const Color GREEN;
		static const Color BLUE;
	public:
		union{
			struct{ float r, g, b, a; };
			float v[4];
		};
		
		Color() :
			r(0.f), g(0.f), b(0.f), a(1.f) {}
		Color(float gray, float a = 1.f) :
			r(gray), g(gray), b(gray), a(a) {}
		Color(float r, float g, float b, float a = 1.f) : r(r), g(g), b(b), a(a) {}
		Color(const Color& ot) :
			r(ot.r), g(ot.g), b(ot.b), a(ot.a){}
		~Color(){}

		static Color RGBA(uint32 code);
		static Color RGB(uint32 code);

		inline Color& operator = (const Color& ot) { r = ot.r, g = ot.g, b = ot.b, a = ot.a; return *this; }
		inline const float& operator [] (size_t i) const { return v[i]; }
		inline       float& operator [] (size_t i)       { return v[i]; }

		inline Color operator + (const Color& ot) const { return Color(r + ot.r, g + ot.g, b + ot.b); }
		inline Color operator - (const Color& ot) const { return Color(r - ot.r, g - ot.g, b - ot.b); }
		inline Color operator * (const Color& ot) const { return Color(r * ot.r, g * ot.g, b * ot.b); }
		inline Color operator * (const float s) const { return Color(r * s, g * s, b * s); }
		inline Color operator / (const float d) const { return Color(r / d, g / d, b / d); }
		inline Color& operator += (const Color& ot) { r += ot.r; g += ot.g; b += ot.b; return *this; }
		inline Color& operator -= (const Color& ot) { r -= ot.r; g -= ot.g; b -= ot.b; return *this; }
		inline Color& operator *= (const Color& ot) { r *= ot.r; g *= ot.g; b *= ot.b; return *this; }
		inline Color& operator *= (const float s) { r *= s; g *= s; b *= s; return *this; }
		inline Color& operator /= (const float d) { r /= d; g /= d; b /= d; return *this; }

		inline bool operator == (const Color ot) const { return r == ot.r && g == ot.g && b == ot.b && a == ot.a; }
		inline bool operator != (const Color ot) const { return r != ot.r || g != ot.g || b != ot.b || a != ot.a; }

		inline bool Valid() const { return Math::Valid(r) && Math::Valid(g) && Math::Valid(b); }
		inline float Luminance() const { return 0.2126f * r + 0.7152f * g + 0.0722f * b; }
		inline Color& Clamp() {
			r = Math::Clamp(r, 0.f, 1.f);
			g = Math::Clamp(g, 0.f, 1.f);
			b = Math::Clamp(b, 0.f, 1.f);
			return *this;
		}
		inline Color Convert(Channel channel, bool keepAlpha = true){
			switch (channel){
			case Channel::Y:
			case Channel::YA:
				return Color(Luminance(), keepAlpha ? a : 1.f);
			case Channel::RGB:
			case Channel::RGBA:
				return Color(r, g, b, keepAlpha ? a : 1.f);
			}
			return Color::BLACK;
		}
	};

	inline std::ostream& operator << (std::ostream& os, const Color& c) {
		return os << "(" << c.r << ", " << c.g << ", " << c.b << ")";
	}

	namespace Math{
		inline Color Exp(const Color& c) { return Color(Exp(c.r), Exp(c.g), Exp(c.g), c.a); }
		inline Color Log(const Color& c) { return Color(Log(c.r), Log(c.g), Log(c.b), c.a); }
	}
}