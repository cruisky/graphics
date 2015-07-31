#pragma once

#define USE_SSE2

#include "Util.h"
#include <intrin.h>
#include "Math/MathUtil.h"
#include "sse_mathfun.h"
#include "Bool.h"

namespace TX {
	namespace SSE {
		struct V4Float {
		public:
			union{
				__m128 m;
				struct { float x, y, z, w; };
				float v[4];
			};
			static const V4Float X;
			static const V4Float Y;
			static const V4Float Z;
			static const V4Float W;
			static const V4Float UNIT[4];
			static const V4Float ZERO;
			static const V4Float ONE;
			static const V4Float PI;
			static const V4Float QTR_PI;
			static const V4Float HALF_PI;
			static const V4Float TWO_PI;
			static const V4Float FOUR_PI;
			static const V4Float PI_RCP;
			static const V4Float HALF_PI_RCP;
			static const V4Float TWO_PI_RCP;
			static const V4Float FOUR_PI_RCP;
			static const V4Float INF;
			static const V4Float EPSILON;
		public:
			__forceinline V4Float() : m(_mm_setzero_ps()){}
			__forceinline V4Float(__m128 d) : m(d){}
			__forceinline V4Float(const float& v) : m(_mm_broadcast_ss(&v)){}
			__forceinline V4Float(float a, float b) : m(_mm_setr_ps(a, b, a, b)){}
			__forceinline V4Float(float a, float b, float c, float d) : m(_mm_setr_ps(a, b, c, d)){}
			__forceinline explicit V4Float(const float *arr) : m(_mm_loadu_ps(arr)){}
			__forceinline V4Float(const V4Float& ot) : m(ot.m){}

			__forceinline operator const __m128&(void) const { return m; }
			__forceinline operator       __m128&(void)       { return m; }

			__forceinline V4Float& operator = (const V4Float& ot){ m = ot.m; return *this; }
			__forceinline const float& operator [] (const size_t i) const { return v[i]; }
			__forceinline		float& operator [] (const size_t i)	      { return v[i]; }
			__forceinline const V4Float operator + () const { return *this; }
			__forceinline const V4Float operator - () const { return _mm_xor_ps(m, _mm_castsi128_ps(SIGN_MASK[0xF])); }

			__forceinline const V4Float operator + (const V4Float& ot) const { return _mm_add_ps(m, ot.m); }
			__forceinline const V4Float operator - (const V4Float& ot) const { return _mm_sub_ps(m, ot.m); }
			__forceinline const V4Float operator * (const V4Float& ot) const { return _mm_mul_ps(m, ot.m); }
			__forceinline const V4Float operator / (const V4Float& ot) const { return *this * _mm_rcp_ps(ot.m); }
			__forceinline const V4Float operator + (float ot) const { return *this + V4Float(ot); }
			__forceinline const V4Float operator - (float ot) const { return *this - V4Float(ot); }
			__forceinline const V4Float operator * (float ot) const { return *this * V4Float(ot); }
			__forceinline const V4Float operator / (float ot) const { return *this * (1.0f / ot); }
			__forceinline V4Float& operator += (const V4Float& ot) { return *this = *this + ot; }
			__forceinline V4Float& operator += (const float& ot) { return *this = *this + ot; }
			__forceinline V4Float& operator -= (const V4Float& ot) { return *this = *this - ot; }
			__forceinline V4Float& operator -= (const float& ot) { return *this = *this - ot; }
			__forceinline V4Float& operator *= (const V4Float& ot) { return *this = *this * ot; }
			__forceinline V4Float& operator *= (const float& ot) { return *this = *this * ot; }
			__forceinline V4Float& operator /= (const V4Float& ot) { return *this = *this / ot; }
			__forceinline V4Float& operator /= (const float& ot) { return *this = *this / ot; }

			__forceinline const bool operator == (const V4Float& ot) const { return _mm_movemask_ps(_mm_cmpeq_ps(m, ot.m)) == 0xF; }
			__forceinline const bool operator == (const float& ot) const { return *this == V4Float(ot); }
			__forceinline const bool operator != (const V4Float& ot) const { return _mm_movemask_ps(_mm_cmpeq_ps(m, ot.m)) != 0xF; }
			__forceinline const bool operator != (const float& ot) const { return *this != V4Float(ot); }

		};
		__forceinline V4Float operator * (float r, const V4Float& v) { return v * r; }
		__forceinline std::ostream& operator << (std::ostream& os, const V4Float& v) {
			return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		}

		__forceinline const V4Float Abs(const V4Float& v){ return _mm_andnot_ps(_mm_castsi128_ps(SIGN_MASK[0xF]), v.m); }
		__forceinline const V4Float Exp(const V4Float& v){ return exp_ps(v.m); }
		__forceinline const V4Float Log(const V4Float& v){ return log_ps(v.m); }
		__forceinline const V4Float Log2(const V4Float& v){ return Log(v) * V4Float(1.4426950408890f); }
		__forceinline const V4Float Log10(const V4Float& v){ return Log(v) * V4Float(0.4342944819033f); }
		__forceinline const V4Float Pow(const V4Float& v, const V4Float& e){ return Exp(e * Log(v)); }

		__forceinline const V4Float ToRad(const V4Float& deg){ return deg * V4Float(0.0055555555555f) * V4Float::PI; }
		__forceinline const V4Float ToDeg(const V4Float& rad){ return rad * V4Float(180.f) * V4Float::PI_RCP; }
		__forceinline const V4Float Sin(V4Float& rad){ return sin_ps(rad.m); }
		__forceinline const V4Float Cos(V4Float& rad){ return cos_ps(rad.m); }
		__forceinline const V4Float Tan(V4Float& rad){ return Sin(rad) / Cos(rad); }

		__forceinline const V4Float Min(const V4Float& a, const V4Float& b){ return _mm_min_ps(a, b); }
		__forceinline const V4Float Max(const V4Float& a, const V4Float& b){ return _mm_max_ps(a, b); }
		__forceinline const V4Float Floor(const V4Float& v){ return _mm_round_ps(v.m, _MM_FROUND_TO_NEG_INF); }
		__forceinline const V4Float Ceil(const V4Float& v){ return _mm_round_ps(v.m, _MM_FROUND_TO_POS_INF); }
		__forceinline const V4Float Round(const V4Float& v){ return _mm_round_ps(v.m, _MM_FROUND_TO_NEAREST_INT); }

		template<bool n0, bool n1, bool n2, bool n3>
		__forceinline const V4Float Negate(const V4Float& v){ return _mm_xor_ps(v.m, SSE::SIGN_MASK[(n3 << 3) | (n2 << 2) | (n1 << 1) | n0]); }

		__forceinline const V4Float Blend(const V4Float& a, const V4Float& b, const V4Bool& fb){ return _mm_blendv_ps(a.m, b.m, _mm_castsi128_ps(fb)); }
		__forceinline const V4Float UnpackLow(const V4Float& a, const V4Float& b){ return _mm_unpacklo_ps(a.m, b.m); }
		__forceinline const V4Float UnpackHigh(const V4Float& a, const V4Float& b){ return _mm_unpackhi_ps(a.m, b.m); }

		template<size_t v0, size_t v1, size_t v2, size_t v3>
		__forceinline const V4Float Shuffle(const V4Float& v) { return _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v), _MM_SHUFFLE(v3, v2, v1, v0))); }
		template<size_t a0, size_t a1, size_t b2, size_t b3>
		__forceinline const V4Float Shuffle(const V4Float& a, const V4Float& b) { return _mm_shuffle_ps(a, b, _MM_SHUFFLE(b3, b2, a1, a0)); }
	}
}