#pragma once

#include "Util.h"
#include <intrin.h>
#include "sse_mathfun.h"

namespace TX
{
	namespace SIMD {
		extern const __m128 SIGN_ALL;	// sign mask for all four elements
		extern const __m128 SIGN_0;		// sign mask for element 0 only
		extern const __m128 SIGN_1;
		extern const __m128 SIGN_2;
		extern const __m128 SIGN_3;

		class Float4 {
		public:
			union { __m128 m128; float f[4]; };
			static const Float4 ZERO;
			static const Float4 ONE;
			static const Float4 PI;
			static const Float4 QTR_PI;
			static const Float4 HALF_PI;
			static const Float4 TWO_PI;
			static const Float4 FOUR_PI;
			static const Float4 PI_RCP;
			static const Float4 HALF_PI_RCP;
			static const Float4 TWO_PI_RCP;
			static const Float4 FOUR_PI_RCP;
		public:
			__forceinline Float4() : m128(_mm_setzero_ps()){}
			__forceinline Float4(const __m128& d) : m128(d){}
			__forceinline Float4(const float& v) : m128(_mm_set_ps1(v)){}
			__forceinline Float4(float a, float b) : m128(_mm_set_ps(b, a, b, a)){}
			__forceinline Float4(float a, float b, float c, float d) : m128(_mm_set_ps(d, c, b, a)){}
			__forceinline explicit Float4(const float *arr) : m128(_mm_loadu_ps(arr)){}
			__forceinline Float4(const Float4& ot) : m128(ot.m128){}
			
			__forceinline operator const __m128&(void) const { return m128; }
			__forceinline operator __m128&(void) { return m128; }

			__forceinline Float4& operator = (const Float4& ot){ m128 = ot.m128; return *this; }
			__forceinline const float& operator [] (const size_t i) const { return f[i]; }
			__forceinline		float& operator [] (const size_t i)	{ return f[i]; }
			__forceinline const Float4 operator + () const { return *this; }
			__forceinline const Float4 operator - () const { return _mm_xor_ps(m128, SIGN_ALL); }

			__forceinline const Float4 operator + (const Float4& rhs) const { return _mm_add_ps(m128, rhs.m128); }
			__forceinline const Float4 operator + (const float& rhs) const { return *this + Float4(rhs); }
			__forceinline const Float4 operator - (const Float4& rhs) const { return _mm_sub_ps(m128, rhs.m128); }
			__forceinline const Float4 operator - (const float& rhs) const { return *this - Float4(rhs); }
			__forceinline const Float4 operator * (const Float4& rhs) const { return _mm_mul_ps(m128, rhs.m128); }
			__forceinline const Float4 operator * (const float& rhs) const { return *this * Float4(rhs); }
			__forceinline const Float4 operator / (const Float4& rhs) const { return *this * _mm_rcp_ps(rhs.m128); }
			__forceinline const Float4 operator / (const float& rhs) const { return *this * (1.0f / rhs); }
			__forceinline Float4& operator += (const Float4& rhs) { return *this = *this + rhs; }
			__forceinline Float4& operator += (const float& rhs) { return *this = *this + rhs; }
			__forceinline Float4& operator -= (const Float4& rhs) { return *this = *this - rhs; }
			__forceinline Float4& operator -= (const float& rhs) { return *this = *this - rhs; }
			__forceinline Float4& operator *= (const Float4& rhs) { return *this = *this * rhs; }
			__forceinline Float4& operator *= (const float& rhs) { return *this = *this * rhs; }
			__forceinline Float4& operator /= (const Float4& rhs) { return *this = *this / rhs; }
			__forceinline Float4& operator /= (const float& rhs) { return *this = *this / rhs; }

			__forceinline const bool operator == (const Float4& rhs) const { return _mm_movemask_ps(_mm_cmpeq_ps(m128, rhs.m128)) == 0xF; }
			__forceinline const bool operator == (const float& rhs) const { return *this == Float4(rhs); }
			__forceinline const bool operator != (const Float4& rhs) const { return _mm_movemask_ps(_mm_cmpeq_ps(m128, rhs.m128)) != 0xF; }
			__forceinline const bool operator != (const float& rhs) const { return *this != Float4(rhs); }

		};
		inline std::ostream& operator << (std::ostream& os, const Float4& v) {
			return os << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")";
		}

		__forceinline const Float4 Abs(const Float4& v){ return _mm_andnot_ps(SIGN_ALL, v.m128); }
		__forceinline const Float4 Exp(const Float4& v){ return exp_ps(v.m128); }
		__forceinline const Float4 Log(const Float4& v){ return log_ps(v.m128); }
		__forceinline const Float4 Log2(const Float4& v){ return Log(v) * Float4(1.4426950408890f); }
		__forceinline const Float4 Log10(const Float4& v){ return Log(v) * Float4(0.4342944819033f); }
		__forceinline const Float4 Pow(const Float4& v, const Float4& e){ return Exp(e * Log(v)); }

		__forceinline const Float4 ToRad(const Float4& deg){ return deg * Float4(0.0055555555555f) * Float4::PI; }
		__forceinline const Float4 ToDeg(const Float4& rad){ return rad * Float4(180.f) * Float4::PI_RCP; }
		__forceinline const Float4 Sin(Float4& rad){ return sin_ps(rad.m128); }
		__forceinline const Float4 Cos(Float4& rad){ return cos_ps(rad.m128); }
		__forceinline const Float4 Tan(Float4& rad){ return Sin(rad) / Cos(rad); }

		__forceinline const Float4 Min(const Float4& a, const Float4& b){ return _mm_min_ps(a, b); }
		__forceinline const Float4 Max(const Float4& a, const Float4& b){ return _mm_max_ps(a, b); }
		__forceinline const Float4 Floor(const Float4& v){ return _mm_round_ps(v.m128, _MM_FROUND_TO_NEG_INF); }
		__forceinline const Float4 Ceil(const Float4& v){ return _mm_round_ps(v.m128, _MM_FROUND_TO_POS_INF); }
		__forceinline const Float4 Round(const Float4& v){ return _mm_round_ps(v.m128, _MM_FROUND_TO_NEAREST_INT); }

		template<size_t mask>
		__forceinline const Float4 Blend(const Float4& a, const Float4& b){ return _mm_blend_ps(a.m128, b.m128, mask); }
		__forceinline const Float4 UnpackLow(const Float4& a, const Float4& b){ return _mm_unpacklo_ps(a.m128, b.m128); }
		__forceinline const Float4 UnpackHigh(const Float4& a, const Float4& b){ return _mm_unpackhi_ps(a.m128, b.m128); }
		
		template<size_t dst, size_t src, size_t clr>
		__forceinline const Float4 Insert(const Float4& lhs, const Float4& rhs) { return _mm_insert_ps(lhs, rhs, (dst << 4) | (src << 6) | clr); }
		template<size_t v0, size_t v1, size_t v2, size_t v3> 
		__forceinline const Float4 Shuffle(const Float4& v)	{ return _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(rhs), _MM_SHUFFLE(v3, v2, v1, v0))); }
		template<size_t a0, size_t a1, size_t b2, size_t b3> 
		__forceinline const Float4 Shuffle(const Float4& a, const Float4& b) { return _mm_shuffle_ps(a, b, _MM_SHUFFLE(b3, b2, a1, a0)); }
	}
}