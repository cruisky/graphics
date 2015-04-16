#pragma once

#include "../Util.h"
#include "MathUtil.h"

namespace Cruisky{
	class Color {
	public:
		static const Color BLACK;
		static const Color WHITE;
		static const Color RED;
		static const Color GREEN;
		static const Color BLUE;
	public:
		float r, g, b, a;
		Color() :
			r(0.f), g(0.f), b(0.f), a(1.f) {}
		Color(float gray, float a = 1.f) :
			r(gray), g(gray), b(gray), a(a) {}
		Color(float r, float g, float b, float a = 1.f) : r(r), g(g), b(b), a(a) {}
		Color(const Color& ot) :
			r(ot.r), g(ot.g), b(ot.b), a(ot.a){}
		~Color(){}

		inline Color& operator = (const Color& ot) { r = ot.r, g = ot.g, b = ot.b, a = ot.a; return *this; }

		inline Color operator + (const Color& ot) const { return Color(r + ot.r, g + ot.g, b + ot.b).Clamp(); }
		inline Color operator - (const Color& ot) const { return Color(r - ot.r, g - ot.g, b - ot.b).Clamp(); }
		inline Color operator * (const Color& ot) const { return Color(r * ot.r, g * ot.g, b * ot.b).Clamp(); }
		inline Color operator * (const float s) const { return Color(r * s, g * s, b * s).Clamp(); }
		inline Color operator / (const float d) const { return Color(r / d, g / d, b / d).Clamp(); }
		inline Color& operator += (const Color& ot) { r += ot.r; g += ot.g; b += ot.b; return Clamp(); }
		inline Color& operator -= (const Color& ot) { r -= ot.r; g -= ot.g; b -= ot.b; return Clamp(); }
		inline Color& operator *= (const Color& ot) { r *= ot.r; g *= ot.g; b *= ot.b; return Clamp(); }
		inline Color& operator *= (const float s) { r *= s; g *= s; b *= s; return Clamp(); }
		inline Color& operator /= (const float d) { r /= d; g /= d; b /= d; return Clamp(); }


		inline bool operator == (const Color ot) const { return r == ot.r && g == ot.g && b == ot.b && a == ot.a; }
		inline bool operator != (const Color ot) const { return r != ot.r || g != ot.g || b != ot.b || a != ot.a; }

		inline float Luminance() const { return 0.2126f * r + 0.7152f * g + 0.0722f * b; }
		inline Color& Clamp() {
			r = Math::Clamp(r, 0.f, 1.f);
			g = Math::Clamp(g, 0.f, 1.f);
			b = Math::Clamp(b, 0.f, 1.f);
			return *this;
		}
	};

	namespace Math{
		inline Color Exp(const Color& c) { return Color(Exp(c.r), Exp(c.g), Exp(c.g), c.a); }
		inline Color Log(const Color& c) { return Color(Log(c.r), Log(c.g), Log(c.b), c.a); }
	}
}