#pragma once

#include "txbase/fwddecl.h"
#include <intrin.h>

#define LOOKUP_TABLE_PS(M) {	\
	_mm_castsi128_ps(_mm_set_epi32(0, 0, 0, 0)),	\
	_mm_castsi128_ps(_mm_set_epi32(0, 0, 0, M)),	\
	_mm_castsi128_ps(_mm_set_epi32(0, 0, M, 0)),	\
	_mm_castsi128_ps(_mm_set_epi32(0, 0, M, M)),	\
	_mm_castsi128_ps(_mm_set_epi32(0, M, 0, 0)),	\
	_mm_castsi128_ps(_mm_set_epi32(0, M, 0, M)),	\
	_mm_castsi128_ps(_mm_set_epi32(0, M, M, 0)),	\
	_mm_castsi128_ps(_mm_set_epi32(0, M, M, M)),	\
	_mm_castsi128_ps(_mm_set_epi32(M, 0, 0, 0)),	\
	_mm_castsi128_ps(_mm_set_epi32(M, 0, 0, M)),	\
	_mm_castsi128_ps(_mm_set_epi32(M, 0, M, 0)),	\
	_mm_castsi128_ps(_mm_set_epi32(M, 0, M, M)),	\
	_mm_castsi128_ps(_mm_set_epi32(M, M, 0, 0)),	\
	_mm_castsi128_ps(_mm_set_epi32(M, M, 0, M)),	\
	_mm_castsi128_ps(_mm_set_epi32(M, M, M, 0)),	\
	_mm_castsi128_ps(_mm_set_epi32(M, M, M, M))}

namespace TX
{
	#if defined(WIN32)
		inline int __bsf(int v) { unsigned long r = 0; _BitScanForward(&r, v); return r; }
		inline int __bsr(int v) { unsigned long r = 0; _BitScanReverse(&r, v); return r; }
		inline int __btc(int v, int i) { long r = v; _bittestandcomplement(&r, i); return r; }
		inline int __bts(int v, int i) { long r = v; _bittestandset(&r, i); return r; }
		inline int __btr(int v, int i) { long r = v; _bittestandreset(&r, i); return r; }
	#endif
	#if defined(_WIN64)
		inline size_t __bsf(size_t v) { size_t r = 0; _BitScanForward64((unsigned long*)&r, v); return r; }
		inline size_t __bsr(size_t v) { size_t r = 0; _BitScanReverse64((unsigned long*)&r, v); return r; }
		inline size_t __btc(size_t v, size_t i) { return v ^ (size_t(1) << i); }
		inline size_t __bts(size_t v, size_t i) { __int64 r = v; _bittestandset64(&r, i); return r; }
		inline size_t __btr(size_t v, size_t i) { __int64 r = v; _bittestandreset64(&r, i); return r; }
	#endif

	namespace SSE
	{
		const __m128 SIGN_MASK[16] = LOOKUP_TABLE_PS(0x80000000);
		const __m128 ELEM_MASK[16] = LOOKUP_TABLE_PS(0xFFFFFFFF);

		struct V4Bool {
		public:
			union{
				__m128 m;
				struct{ int32 x, y, z, w; };
				int32 v[4];
			};
		public:
			inline V4Bool() :m(_mm_setzero_ps()) {}
			inline V4Bool(__m128i mi) : m(_mm_castsi128_ps(mi)) {}
			inline V4Bool(__m128 mf) : m(mf) {}
			inline V4Bool(const V4Bool& ot) : m(ot.m) {}
			inline V4Bool(bool a) : m(ELEM_MASK[(int(a) << 3) | (int(a) << 2) | (int(a) << 1) | int(a)]){}
			inline V4Bool(bool a, bool b) : m(ELEM_MASK[(int(b) << 3) | (int(a) << 2) | (int(b) << 1) | int(a)]){}
			inline V4Bool(bool a, bool b, bool c, bool d) : m(ELEM_MASK[(int(d) << 3) | (int(c) << 2) | (int(b) << 1) | int(a)]){}

			inline V4Bool& operator = (const V4Bool& ot){ m = ot.m; return *this; }

			inline operator const __m128&(void) const { return m; }
			inline operator       __m128&(void)       { return m; }

			inline bool   operator [] (const size_t i) const { return (_mm_movemask_ps(m) >> i) & 1; }
			inline int32& operator [] (const size_t i)       { return v[i]; }

			inline const V4Bool operator ! () const { return _mm_xor_ps(*this, V4Bool(true)); }
			inline const V4Bool operator & (const V4Bool& ot) const { return _mm_and_ps(*this, ot); }
			inline const V4Bool operator | (const V4Bool& ot) const { return _mm_or_ps(*this, ot); }
			inline const V4Bool operator ^ (const V4Bool& ot) const { return _mm_xor_ps(*this, ot); }
			inline const V4Bool operator &= (const V4Bool& ot) { return *this = *this & ot; }
			inline const V4Bool operator |= (const V4Bool& ot) { return *this = *this | ot; }
			inline const V4Bool operator ^= (const V4Bool& ot) { return *this = *this ^ ot; }
			inline const V4Bool operator != (const V4Bool& ot) const { return _mm_xor_ps(*this, ot); }
			inline const V4Bool operator == (const V4Bool& ot) const { return _mm_cmpeq_ps(*this, ot); }
		};

		inline std::ostream& operator << (std::ostream& os, const V4Bool& v) {
			return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		}

		inline V4Bool UnpackLow(const V4Bool& a, const V4Bool& b) { return _mm_unpacklo_ps(a.m, b.m); }
		inline V4Bool UnpackHigh(const V4Bool& a, const V4Bool& b) { return _mm_unpackhi_ps(a.m, b.m); }

		template<size_t v0, size_t v1, size_t v2, size_t v3>
		inline const V4Bool Shuffle(const V4Bool& a){ return _mm_shuffle_epi32(a, _MM_SHUFFLE(v3, v2, v1, v0)); }
		template<size_t a0, size_t a1, size_t b2, size_t b3>
		inline const V4Bool Shuffle(const V4Bool& a, const V4Bool& b){ return _mm_shuffle_ps(a, b, _MM_SHUFFLE(b3, b2, a1, a0)); }

		inline bool All(const V4Bool& a) { return _mm_movemask_ps(a) == 0xf; }
		inline bool Any(const V4Bool& a) { return _mm_movemask_ps(a) != 0x0; }
		inline bool None(const V4Bool& a) { return _mm_movemask_ps(a) == 0x0; }
	}
}
