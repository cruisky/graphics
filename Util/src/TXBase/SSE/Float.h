#pragma once

#define USE_SSE2

#include <intrin.h>
#include "TXBase/fwddecl.h"
#include "TXBase/Math/Base.h"
#include "TXBase/Libs/sse_mathfun.h"
#include "Bool.h"
#include "Int.h"

namespace TX {
	namespace SSE {
		struct V4Float {
		public:
			union {
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
			static const V4Float PI_RCP;
			static const V4Float INF;
			static const V4Float EPSILON;
		public:
			inline V4Float() : m(_mm_setzero_ps()) {}
			inline V4Float(__m128 d) : m(d) {}
			inline V4Float(const float& v) : m(_mm_broadcast_ss(&v)) {}
			inline V4Float(float a, float b) : m(_mm_setr_ps(a, b, a, b)) {}
			inline V4Float(float a, float b, float c, float d) : m(_mm_setr_ps(a, b, c, d)) {}
			inline explicit V4Float(const float *arr) : m(_mm_loadu_ps(arr)) {}
			inline V4Float(const V4Float& ot) : m(ot.m) {}

			inline operator const __m128&(void) const { return m; }
			inline operator       __m128&(void) { return m; }

			inline V4Float& operator = (const V4Float& ot) { m = ot.m; return *this; }
			inline const float& operator [] (const size_t i) const { return v[i]; }
			inline		 float& operator [] (const size_t i)       { return v[i]; }
			inline const V4Float operator + () const { return *this; }
			inline const V4Float operator - () const { return _mm_xor_ps(m, SIGN_MASK[0xF]); }

			inline const V4Float operator + (const V4Float& ot) const { return _mm_add_ps(m, ot.m); }
			inline const V4Float operator - (const V4Float& ot) const { return _mm_sub_ps(m, ot.m); }
			inline const V4Float operator * (const V4Float& ot) const { return _mm_mul_ps(m, ot.m); }
			inline const V4Float operator / (const V4Float& ot) const { return *this * _mm_rcp_ps(ot.m); }
			inline const V4Float operator + (float ot) const { return *this + V4Float(ot); }
			inline const V4Float operator - (float ot) const { return *this - V4Float(ot); }
			inline const V4Float operator * (float ot) const { return *this * V4Float(ot); }
			inline const V4Float operator / (float ot) const { return *this * (1.0f / ot); }
			inline V4Float& operator += (const V4Float& ot) { return *this = *this + ot; }
			inline V4Float& operator += (const float& ot) { return *this = *this + ot; }
			inline V4Float& operator -= (const V4Float& ot) { return *this = *this - ot; }
			inline V4Float& operator -= (const float& ot) { return *this = *this - ot; }
			inline V4Float& operator *= (const V4Float& ot) { return *this = *this * ot; }
			inline V4Float& operator *= (const float& ot) { return *this = *this * ot; }
			inline V4Float& operator /= (const V4Float& ot) { return *this = *this / ot; }
			inline V4Float& operator /= (const float& ot) { return *this = *this / ot; }

			inline const V4Bool operator == (const V4Float& ot) const { return _mm_cmpeq_ps(m, ot.m); }
			inline const V4Bool operator == (const float& ot) const { return *this == V4Float(ot); }
			inline const V4Bool operator != (const V4Float& ot) const { return _mm_cmpneq_ps(m, ot.m); }
			inline const V4Bool operator != (const float& ot) const { return *this != V4Float(ot); }
			inline const V4Bool operator < (const V4Float& ot) const { return _mm_cmplt_ps(m, ot.m); }
			inline const V4Bool operator < (const float& ot) const { return *this < V4Float(ot); }
			inline const V4Bool operator > (const V4Float& ot) const { return _mm_cmpgt_ps(m, ot.m); }
			inline const V4Bool operator > (const float& ot) const { return *this > V4Float(ot); }
			inline const V4Bool operator <= (const V4Float& ot) const { return _mm_cmple_ps(m, ot.m); }
			inline const V4Bool operator <= (const float& ot) const { return *this <= V4Float(ot); }
			inline const V4Bool operator >= (const V4Float& ot) const { return _mm_cmpge_ps(m, ot.m); }
			inline const V4Bool operator >= (const float& ot) const { return *this >= V4Float(ot); }

			inline const V4Float operator & (const V4Float& ot) const { return _mm_and_ps(m, ot.m); }
			inline const V4Float operator & (const V4Int& mask) const { return _mm_and_ps(m, _mm_castsi128_ps(mask.m)); }
			inline const V4Float operator | (const V4Float& ot) const { return _mm_or_ps(m, ot.m); }
			inline const V4Float operator | (const V4Int& mask) const { return _mm_or_ps(m, _mm_castsi128_ps(mask.m)); }
			inline const V4Float operator ^ (const V4Float& ot) const { return _mm_xor_ps(m, ot.m); }
			inline const V4Float operator ^ (const V4Int& mask) const { return _mm_xor_ps(m, _mm_castsi128_ps(mask.m)); }
		};
		inline V4Float operator * (float r, const V4Float& v) { return v * r; }
		inline std::ostream& operator << (std::ostream& os, const V4Float& v) {
			return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		}

		inline const V4Float Abs(const V4Float& v) { return _mm_andnot_ps(SIGN_MASK[0xF], v.m); }
		inline const V4Float Exp(const V4Float& v) { return exp_ps(v.m); }
		inline const V4Float Log(const V4Float& v) { return log_ps(v.m); }
		inline const V4Float Log2(const V4Float& v) { return Log(v) * V4Float(1.4426950408890f); }
		inline const V4Float Log10(const V4Float& v) { return Log(v) * V4Float(0.4342944819033f); }
		inline const V4Float Pow(const V4Float& v, const V4Float& e) { return Exp(e * Log(v)); }

		inline const V4Float ToRad(const V4Float& deg) { return deg * V4Float(0.0055555555555f) * V4Float::PI; }
		inline const V4Float ToDeg(const V4Float& rad) { return rad * V4Float(180.f) * V4Float::PI_RCP; }
		inline const V4Float Sin(V4Float& rad) { return sin_ps(rad.m); }
		inline const V4Float Cos(V4Float& rad) { return cos_ps(rad.m); }
		inline const V4Float Tan(V4Float& rad) { return Sin(rad) / Cos(rad); }

		inline const V4Float Min(const V4Float& a, const V4Float& b) { return _mm_min_ps(a, b); }
		inline const V4Float Max(const V4Float& a, const V4Float& b) { return _mm_max_ps(a, b); }
		inline const V4Float Floor(const V4Float& v) { return _mm_round_ps(v.m, _MM_FROUND_TO_NEG_INF); }
		inline const V4Float Ceil(const V4Float& v) { return _mm_round_ps(v.m, _MM_FROUND_TO_POS_INF); }
		inline const V4Float Round(const V4Float& v) { return _mm_round_ps(v.m, _MM_FROUND_TO_NEAREST_INT); }
		template<bool n0, bool n1, bool n2, bool n3>
		inline const V4Float Negate(const V4Float& v) { return _mm_xor_ps(v.m, SSE::SIGN_MASK[(n3 << 3) | (n2 << 2) | (n1 << 1) | n0]); }

		inline const V4Float Select(const V4Bool& maska, const V4Float& a, const V4Float& b) { return _mm_blendv_ps(b.m, a.m, maska); }
		inline const V4Float UnpackLow(const V4Float& a, const V4Float& b) { return _mm_unpacklo_ps(a.m, b.m); }
		inline const V4Float UnpackHigh(const V4Float& a, const V4Float& b) { return _mm_unpackhi_ps(a.m, b.m); }
		template<size_t v0, size_t v1, size_t v2, size_t v3>
		inline const V4Float Shuffle(const V4Float& v) { return _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v), _MM_SHUFFLE(v3, v2, v1, v0))); }
		template<size_t a0, size_t a1, size_t b2, size_t b3>
		inline const V4Float Shuffle(const V4Float& a, const V4Float& b) { return _mm_shuffle_ps(a, b, _MM_SHUFFLE(b3, b2, a1, a0)); }

		inline const V4Float VReduceMin(const V4Float& v) { V4Float h = Min(Shuffle<1, 0, 3, 2>(v), v); return Min(Shuffle<2, 3, 0, 1>(h), h); }
		inline const V4Float VReduceMax(const V4Float& v) { V4Float h = Max(Shuffle<1, 0, 3, 2>(v), v); return Max(Shuffle<2, 3, 0, 1>(h), h); }
		inline const V4Float VReduceAdd(const V4Float& v) { V4Float h = Shuffle<1, 0, 3, 2>(v) + v; return Shuffle<2, 3, 0, 1>(h) + h; }
		inline const float ReduceMin(const V4Float& v) { return _mm_cvtss_f32(VReduceMin(v)); }
		inline const float ReduceMax(const V4Float& v) { return _mm_cvtss_f32(VReduceMax(v)); }
		inline const float ReduceAdd(const V4Float& v) { return _mm_cvtss_f32(VReduceAdd(v)); }
		inline size_t SelectMin(const V4Float& v) {
			auto r = VReduceMin(v);
			auto e = v == r;
			int mask = _mm_movemask_ps(e);
			return __bsf(mask);
			//return __bsf(_mm_movemask_epi8(v == VReduceMin(v)));
		}
		inline size_t SelectMax(const V4Float& v) { return __bsf(_mm_movemask_ps(v == VReduceMax(v))); }
		inline size_t SelectMin(const V4Bool& valid, const V4Float& v) { const V4Float f = Select(valid, v, V4Float::INF); return __bsf(_mm_movemask_ps(valid & (f == VReduceMin(f)))); }
		inline size_t SelectMax(const V4Bool& valid, const V4Float& v) { const V4Float f = Select(valid, v, -V4Float::INF); return __bsf(_mm_movemask_ps(valid & (f == VReduceMax(f)))); }
	}
}
