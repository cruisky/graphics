#pragma once

#include <cmath>

namespace TX {
	namespace Math {
		///////////////////////////////////////////////////////////////////////
		// Constants
		///////////////////////////////////////////////////////////////////////
		static struct {
			template <typename T> constexpr operator T() const { return T(0); }
		} ZERO;
		static struct {
			template <typename T> constexpr operator T() const { return T(1); }
		} ONE;
		static struct {
			constexpr operator float() const { return 3.14159265358979323846f; }
		} PI;
		static struct {
			constexpr operator float() const { return 0.31830988618379069122f; }
		} PI_RCP;
		static struct {
			inline operator float() const { return std::numeric_limits<float>::quiet_NaN(); }
		} NaN;
		static struct {
			constexpr operator float() const { return std::numeric_limits<float>::infinity(); }
		} INF;
		static struct {
			constexpr operator float() const { return std::numeric_limits<float>::epsilon(); }
		} EPSILON;

		///////////////////////////////////////////////////////////////////////
		// Functions
		///////////////////////////////////////////////////////////////////////
		template <typename T>
		inline bool IsNAN(const T& num) { return _isnan(num); }
		template <typename T>
		inline bool IsINF(const T& num) { return !_finite(num); }
		template <typename T>
		inline bool Valid(const T& num) { return !_isnan(num) && _finite(num); }

		template <typename T>
		inline T     Abs(T n) { return std::abs(n); }
		inline float Exp(float n) { return expf(n); }
		inline float Log(float n) { return logf(n); }
		inline float Log2(float n) { return log2f(n); }
		inline float Log10(float n) { return log10f(n); }
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

		inline float ToRad(float deg) { return deg / 180.f * Math::PI; }
		inline float ToDeg(float rad) { return rad * 180.f / Math::PI; }
		inline float Sin(float r) { return sinf(r); }
		inline float Cos(float r) { return cosf(r); }
		inline float Tan(float r) { return tanf(r); }
		inline float Asin(float s) { return asinf(s); }
		inline float Acos(float c) { return acosf(c); }
		inline float Atan2(float y, float x) { return atan2f(y, x); }

		template <typename T1, typename T2> inline auto Max(const T1& n1, const T2& n2) -> decltype(n1 + n2) { return n1 > n2 ? n1 : n2; }
		template <typename T1, typename T2> inline auto Min(const T1& n1, const T2& n2) -> decltype(n1 + n2) { return n1 < n2 ? n1 : n2; }
		template <typename T> inline int Sign(const T& n){ return (T(0) < val) - (val < T(0)); }
		template <typename T> inline bool InBounds(const T& n, const T& min, const T& max) { return min <= n && n <= max; }
		template <typename T> inline T Clamp(const T& val, const T& min, const T& max){
			return (val > min) ? ((val < max) ? val : max) : min;
		}
		inline int Floor(float n) { int t = int(n); return t - (t > n); }
		inline int Ceil(float n) { int t = int(n); return t + (t < n); }
		inline int Round(float n) { return n > 0 ? Floor(n + 0.5f) : Ceil(n - 0.5f); }
		inline float Lerp(float t, float v1, float v2){ return (1.f - t) * v1 + t * v2; }
		inline float InvLerp(float v, float v1, float v2){ return (v - v1) / (v2 - v1); }
	}
}