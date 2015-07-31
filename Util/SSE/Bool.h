#pragma once

#include "Util.h"
#include <intrin.h>

#define LOOKUP_TABLE_EPI32(M) {	\
	_mm_set_epi32(0, 0, 0, 0),	\
	_mm_set_epi32(0, 0, 0, M),	\
	_mm_set_epi32(0, 0, M, 0),	\
	_mm_set_epi32(0, 0, M, M),	\
	_mm_set_epi32(0, M, 0, 0),	\
	_mm_set_epi32(0, M, 0, M),	\
	_mm_set_epi32(0, M, M, 0),	\
	_mm_set_epi32(0, M, M, M),	\
	_mm_set_epi32(M, 0, 0, 0),	\
	_mm_set_epi32(M, 0, 0, M),	\
	_mm_set_epi32(M, 0, M, 0),	\
	_mm_set_epi32(M, 0, M, M),	\
	_mm_set_epi32(M, M, 0, 0),	\
	_mm_set_epi32(M, M, 0, M),	\
	_mm_set_epi32(M, M, M, 0),	\
	_mm_set_epi32(M, M, M, M)}

namespace TX
{
	namespace SSE
	{
		const __m128i SIGN_MASK[16] = LOOKUP_TABLE_EPI32(0x80000000);
		const __m128i ELEM_MASK[16] = LOOKUP_TABLE_EPI32(0xFFFFFFFF);

		struct V4Bool {
		public:
			union{
				__m128i m;
				struct{ int32 x, y, z, w; };
				int32 v[4];
			};
			static const V4Bool TRUE;
			static const V4Bool FALSE;
		public:
			__forceinline V4Bool() :m(_mm_setzero_si128()) {}
			__forceinline V4Bool(__m128i mi) : m(mi) {}
			__forceinline V4Bool(const V4Bool& ot) : m(ot.m) {}
			__forceinline V4Bool(bool a) : m(ELEM_MASK[(int(a) << 3) | (int(a) << 2) | (int(a) << 1) | int(a)]){}
			__forceinline V4Bool(bool a, bool b) : m(ELEM_MASK[(int(b) << 3) | (int(a) << 2) | (int(b) << 1) | int(a)]){}
			__forceinline V4Bool(bool a, bool b, bool c, bool d) : m(ELEM_MASK[(int(d) << 3) | (int(c) << 2) | (int(b) << 1) | int(a)]){}
			
			__forceinline V4Bool& operator = (const V4Bool& ot){ m = ot.m; return *this; }

			__forceinline operator const __m128i&(void) const { return m; }
			__forceinline operator       __m128i&(void)       { return m; }

			__forceinline bool   operator [] (const size_t i) const { return (_mm_movemask_ps(_mm_castsi128_ps(m)) >> i) & 1; }
			__forceinline int32& operator [] (const size_t i)       { return v[i]; }

			__forceinline const V4Bool operator ! () const { return _mm_xor_si128(*this, V4Bool::TRUE); }
			__forceinline const V4Bool operator & (const V4Bool& ot) const { return _mm_and_si128(*this, ot); }
			__forceinline const V4Bool operator | (const V4Bool& ot) const { return _mm_or_si128(*this, ot); }
			__forceinline const V4Bool operator ^ (const V4Bool& ot) const { return _mm_xor_si128(*this, ot); }
			__forceinline const V4Bool operator &= (const V4Bool& ot) { return *this = *this & ot; }
			__forceinline const V4Bool operator |= (const V4Bool& ot) { return *this = *this | ot; }
			__forceinline const V4Bool operator ^= (const V4Bool& ot) { return *this = *this ^ ot; }
			__forceinline const V4Bool operator != (const V4Bool& ot) const { return _mm_xor_si128(*this, ot); }
			__forceinline const V4Bool operator == (const V4Bool& ot) const { return _mm_cmpeq_epi32(*this, ot); }
		};

		__forceinline V4Bool UnpackLow(const V4Bool& a, const V4Bool& b) { return _mm_unpacklo_epi32(a.m, b.m); }
		__forceinline V4Bool UnpackHigh(const V4Bool& a, const V4Bool& b) { return _mm_unpackhi_epi32(a.m, b.m); }
		
		template<size_t v0, size_t v1, size_t v2, size_t v3>
		__forceinline const V4Bool Shuffle(const V4Bool& v){ return _mm_shuffle_epi32(lhs, _MM_SHUFFLE(v3, v2, v1, v0)); }
		template<size_t a0, size_t a1, size_t b2, size_t b3>
		__forceinline const V4Bool Shuffle(const V4Bool& a, const V4Bool& b){ return _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(lhs), _mm_castsi128_ps(rhs), _MM_SHUFFLE(b3, b2, a1, a0))); }

		__forceinline bool All(const V4Bool& a) { return _mm_movemask_ps(_mm_castsi128_ps(a)) == 0xf; }
		__forceinline bool Any(const V4Bool& a) { return _mm_movemask_ps(_mm_castsi128_ps(a)) != 0x0; }
		__forceinline bool None(const V4Bool& a) { return _mm_movemask_ps(_mm_castsi128_ps(a)) == 0x0; }
	}
}
