#pragma once

#ifdef UTIL_EXPORTS
#define UTIL_API __declspec(dllexport) 
#else
#define UTIL_API __declspec(dllimport) 
#endif

#include <cmath>

namespace Cruisky {
	namespace Math {
		extern UTIL_API const float PI;
		extern UTIL_API const float INF;
		extern UTIL_API const float EPSILON;

		template <typename T>
		UTIL_API inline bool IsNAN(const T& num) { return _isnan(num); }
		template <typename T>
		UTIL_API inline bool IsINF(const T& num) { return !_finite(num); }
		template <typename T>
		UTIL_API inline bool Valid(const T& num) { return !_isnan(num) && _finite(num); }

		UTIL_API inline float Abs(float n) { return fabsf(n); }
		UTIL_API inline float Pow(float n, float e) { return powf(n, e); }
		UTIL_API inline float Sqrt(float n) { return sqrtf(n); }
		UTIL_API inline float Rsqrt(float n) {
			const float threehalfs = 1.5f;
			float x2 = n * 0.5f, y = n;
			long& i = *reinterpret_cast<long *>(&y);
			i = 0x5f375a86 - (i >> 1);
			y *= (threehalfs - (x2 * y * y));
			return y * (threehalfs - (x2 * y * y));
			//return 1.0f / Math::Sqrt(n);
		}

		UTIL_API inline float Sin(float r) { return sinf(r); }
		UTIL_API inline float Cos(float r) { return cosf(r); }
		UTIL_API inline float Tan(float r) { return tanf(r); }
		UTIL_API inline float Asin(float s) { return asinf(s); }
		UTIL_API inline float Acos(float c) { return acosf(c); }
		UTIL_API inline float Atan2(float y, float x) { return atan2f(y, x); }
		UTIL_API inline float Exp(float n) { return expf(n); }
		UTIL_API inline float Log(float n) { return logf(n); }
		UTIL_API inline float Log2(float n) { return log2f(n); }
		UTIL_API inline float Log10(float n) { return log10f(n); }
		UTIL_API inline float ToRad(float deg) { return deg / 180.0f * Math::PI; }
		UTIL_API inline float ToDeg(float rad) { return rad * 180.0f / Math::PI; }
		template <typename T1, typename T2> UTIL_API inline auto Max(const T1& n1, const T2 n2) -> decltype(n1 + n2) { return n1 > n2 ? n1 : n2; }
		template <typename T1, typename T2> UTIL_API inline auto Min(const T1& n1, const T2 n2) -> decltype(n1 + n2) { return n1 < n2 ? n1 : n2; }
		template <typename T> inline bool InBounds(const T& n, const T& min, const T& max) { return !(n < min) && !(max < n); }
		template <typename T> inline T Clamp(const T& val, const T& min, const T& max){
			return (val > min) ? ((val < max) ? val : max) : min;
		}
	}
}