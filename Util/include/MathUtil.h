#pragma once
#pragma warning(disable: 4800)

#include <cmath>

namespace Cruisky {
	namespace Math {
		extern const float PI;
		extern const float INF;
		extern const float EPSILON;

		template <typename T>
		inline bool IsNAN(const T& num) { return _isnan(num); }
		template <typename T>
		inline bool IsINF(const T& num) { return !_finite(num); }
		template <typename T>
		inline bool Valid(const T& num) { return !_isnan(num) && _finite(num); }

		inline float Abs(float n) { return fabsf(n); }
		inline float Pow(float n, float e) { return powf(n, e); }
		inline float Sqrt(float n) { return sqrtf(n); }
		inline float Rsqrt(float n) {
			const float threehalfs = 1.5f;
			float x2 = n * 0.5f, y = n;
			long& i = *reinterpret_cast<long *>(&y);
			i = 0x5f375a86 - (i >> 1);
			y *= (threehalfs - (x2 * y * y));
			return y * (threehalfs - (x2 * y * y));
			//return 1.0f / Math::Sqrt(n);
		}

		inline float Sin(float r) { return sinf(r); }
		inline float Cos(float r) { return cosf(r); }
		inline float Tan(float r) { return tanf(r); }
		inline float Asin(float s) { return asinf(s); }
		inline float Acos(float c) { return acosf(c); }
		inline float Atan2(float y, float x) { return atan2f(y, x); }
		inline float Exp(float n) { return expf(n); }
		inline float Log(float n) { return logf(n); }
		inline float Log2(float n) { return log2f(n); }
		inline float Log10(float n) { return log10f(n); }
		inline float ToRad(float deg) { return deg / 180.0f * Math::PI; }
		inline float ToDeg(float rad) { return rad * 180.0f / Math::PI; }
		template <typename T1, typename T2> inline auto Max(const T1& n1, const T2 n2) -> decltype(n1 + n2) { return n1 > n2 ? n1 : n2; }
		template <typename T1, typename T2> inline auto Min(const T1& n1, const T2 n2) -> decltype(n1 + n2) { return n1 < n2 ? n1 : n2; }
		template <typename T> inline bool InBounds(const T& n, const T& min, const T& max) { return !(n < min) && !(max < n); }
		template <typename T> inline T Clamp(const T& val, const T& min, const T& max){
			return (val > min) ? ((val < max) ? val : max) : min;
		}

		inline int Floor(float n) { return (int)(n + EPSILON) - 1; }
		inline int Ceil(float n) { return (int)(n - EPSILON) + 1; }
		inline int Round(float n) { return (int)(n + 0.5f); }
	}
}