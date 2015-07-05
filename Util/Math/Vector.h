#pragma once

#include "Util.h"
#include <intrin.h>
#include "MathUtil.h"
#include "sse_mathfun.h"

#define LOOKUP_TABLE_PS(M) {						\
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

namespace TX {
	namespace SSE {
		const __m128 SIGN_MASK[16] = LOOKUP_TABLE_PS(0x80000000);
		const __m128 ELEM_MASK[16] = LOOKUP_TABLE_PS(0xFFFFFFFF);
	}

	// 16-byte alignment for dynamic allocation, any class that contains a instance of this class should inherit from this class
	struct Align16 {
		void* operator new(size_t size){ return _mm_malloc(size, 16); }
		void operator delete(void * mem) { _mm_free(mem); }
		void* operator new[](size_t size) { return _mm_malloc(size, 16); }
		void operator delete[](void *mem) { _mm_free(mem); }
	};

	// using SSE packed single type
	class Vector4 : public Align16 {
	public:
		union {
			__m128 m128;
			float f[4];
			struct { float x; float y; float z; float w; };
		};
		static const Vector4 X;
		static const Vector4 Y;
		static const Vector4 Z;
		static const Vector4 W;
		static const Vector4 ZERO;
		static const Vector4 ONE;
		static const Vector4 PI;
		static const Vector4 QTR_PI;
		static const Vector4 HALF_PI;
		static const Vector4 TWO_PI;
		static const Vector4 FOUR_PI;
		static const Vector4 PI_RCP;
		static const Vector4 HALF_PI_RCP;
		static const Vector4 TWO_PI_RCP;
		static const Vector4 FOUR_PI_RCP;
	public:
		__forceinline Vector4() : m128(_mm_setzero_ps()){}
		__forceinline Vector4(const __m128& d) : m128(d){}
		__forceinline Vector4(const float& v) : m128(_mm_broadcast_ss(&v)){}
		__forceinline Vector4(float a, float b) : m128(_mm_setr_ps(a, b, a, b)){}
		__forceinline Vector4(float a, float b, float c, float d) : m128(_mm_setr_ps(a, b, c, d)){}
		__forceinline explicit Vector4(const float *arr) : m128(_mm_loadu_ps(arr)){}
		__forceinline Vector4(const Vector4& ot) : m128(ot.m128){}

		__forceinline operator const __m128&(void) const { return m128; }
		__forceinline operator __m128&(void) { return m128; }

		__forceinline void Validate() const { assert(!Math::IsNAN(x)); assert(!Math::IsNAN(y)); assert(!Math::IsNAN(z)); assert(!Math::IsNAN(w)); }

		__forceinline Vector4& operator = (const Vector4& ot){ m128 = ot.m128; return *this; }
		__forceinline const float& operator [] (const size_t i) const { return f[i]; }
		__forceinline		float& operator [] (const size_t i)	{ return f[i]; }
		__forceinline const Vector4 operator + () const { return *this; }
		__forceinline const Vector4 operator - () const { return _mm_xor_ps(m128, SSE::SIGN_MASK[0xF]); }

		__forceinline const Vector4 operator + (const Vector4& rhs) const { return _mm_add_ps(m128, rhs.m128); }
		__forceinline const Vector4 operator - (const Vector4& rhs) const { return _mm_sub_ps(m128, rhs.m128); }
		__forceinline const Vector4 operator * (const Vector4& rhs) const { return _mm_mul_ps(m128, rhs.m128); }
		__forceinline const Vector4 operator / (const Vector4& rhs) const { return *this * _mm_rcp_ps(rhs.m128); }
		__forceinline const Vector4 operator + (float rhs) const { return *this + Vector4(rhs); }
		__forceinline const Vector4 operator - (float rhs) const { return *this - Vector4(rhs); }
		__forceinline const Vector4 operator * (float rhs) const { return *this * Vector4(rhs); }
		__forceinline const Vector4 operator / (float rhs) const { return *this * (1.0f / rhs); }
		__forceinline Vector4& operator += (const Vector4& rhs) { return *this = *this + rhs; }
		__forceinline Vector4& operator += (const float& rhs) { return *this = *this + rhs; }
		__forceinline Vector4& operator -= (const Vector4& rhs) { return *this = *this - rhs; }
		__forceinline Vector4& operator -= (const float& rhs) { return *this = *this - rhs; }
		__forceinline Vector4& operator *= (const Vector4& rhs) { return *this = *this * rhs; }
		__forceinline Vector4& operator *= (const float& rhs) { return *this = *this * rhs; }
		__forceinline Vector4& operator /= (const Vector4& rhs) { return *this = *this / rhs; }
		__forceinline Vector4& operator /= (const float& rhs) { return *this = *this / rhs; }

		__forceinline const bool operator == (const Vector4& rhs) const { return _mm_movemask_ps(_mm_cmpeq_ps(m128, rhs.m128)) == 0xF; }
		__forceinline const bool operator == (const float& rhs) const { return *this == Vector4(rhs); }
		__forceinline const bool operator != (const Vector4& rhs) const { return _mm_movemask_ps(_mm_cmpeq_ps(m128, rhs.m128)) != 0xF; }
		__forceinline const bool operator != (const float& rhs) const { return *this != Vector4(rhs); }

	};
	__forceinline Vector4 operator * (float r, const Vector4& v) { return v * r; }

	inline std::ostream& operator << (std::ostream& os, const Vector4& v) {
		return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	}

	__forceinline const Vector4 Abs(const Vector4& v){ return _mm_andnot_ps(SSE::SIGN_MASK[0xF], v.m128); }
	__forceinline const Vector4 Exp(const Vector4& v){ return exp_ps(v.m128); }
	__forceinline const Vector4 Log(const Vector4& v){ return log_ps(v.m128); }
	__forceinline const Vector4 Log2(const Vector4& v){ return Log(v) * Vector4(1.4426950408890f); }
	__forceinline const Vector4 Log10(const Vector4& v){ return Log(v) * Vector4(0.4342944819033f); }
	__forceinline const Vector4 Pow(const Vector4& v, const Vector4& e){ return Exp(e * Log(v)); }

	__forceinline const Vector4 ToRad(const Vector4& deg){ return deg * Vector4(0.0055555555555f) * Vector4::PI; }
	__forceinline const Vector4 ToDeg(const Vector4& rad){ return rad * Vector4(180.f) * Vector4::PI_RCP; }
	__forceinline const Vector4 Sin(Vector4& rad){ return sin_ps(rad.m128); }
	__forceinline const Vector4 Cos(Vector4& rad){ return cos_ps(rad.m128); }
	__forceinline const Vector4 Tan(Vector4& rad){ return Sin(rad) / Cos(rad); }

	__forceinline const Vector4 Min(const Vector4& a, const Vector4& b){ return _mm_min_ps(a, b); }
	__forceinline const Vector4 Max(const Vector4& a, const Vector4& b){ return _mm_max_ps(a, b); }
	__forceinline const Vector4 Floor(const Vector4& v){ return _mm_round_ps(v.m128, _MM_FROUND_TO_NEG_INF); }
	__forceinline const Vector4 Ceil(const Vector4& v){ return _mm_round_ps(v.m128, _MM_FROUND_TO_POS_INF); }
	__forceinline const Vector4 Round(const Vector4& v){ return _mm_round_ps(v.m128, _MM_FROUND_TO_NEAREST_INT); }

	__forceinline float Dot4(const Vector4& u, const Vector4& v){
		return _mm_cvtss_f32(_mm_dp_ps(u, v, 0xF1));	// dp[0,1,2,3] -> r[0]
	}

	template<bool n0, bool n1, bool n2, bool n3>
	__forceinline const Vector4 Negate(const Vector4& v){ return _mm_xor_ps(v.m128, SSE::SIGN_MASK[(n3 << 3) | (n2 << 2) | (n1 << 1) | n0]); }

	template<size_t b0, size_t b1, size_t b2, size_t b3>
	__forceinline const Vector4 Blend(const Vector4& a, const Vector4& b){ return _mm_blend_ps(a.m128, b.m128, (b3 << 3) | (b2 << 2) | (b1 << 1) | b0); }
	__forceinline const Vector4 UnpackLow(const Vector4& a, const Vector4& b){ return _mm_unpacklo_ps(a.m128, b.m128); }
	__forceinline const Vector4 UnpackHigh(const Vector4& a, const Vector4& b){ return _mm_unpackhi_ps(a.m128, b.m128); }

	template<size_t dst2, size_t src2, size_t clr2>
	__forceinline const Vector4 Insert(const Vector4& dst, const Vector4& src) { return _mm_insert_ps(dst, src, (dst2 << 4) | (src2 << 6) | clr2); }
	template<size_t v0, size_t v1, size_t v2, size_t v3>
	__forceinline const Vector4 Shuffle(const Vector4& v)	{ return _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v), _MM_SHUFFLE(v3, v2, v1, v0))); }
	template<size_t a0, size_t a1, size_t b2, size_t b3>
	__forceinline const Vector4 Shuffle(const Vector4& a, const Vector4& b) { return _mm_shuffle_ps(a, b, _MM_SHUFFLE(b3, b2, a1, a0)); }


	// Vector3 hides the last member of Vector4
	class Vector3 : public Vector4 {
	public:
		static const Vector3 ONE;
	public:
		Vector3(){}
		Vector3(float v) : Vector3(v, v, v){}
		Vector3(float x, float y, float z) : Vector4(x, y, z, 0.f) {}
		Vector3(float arr[3]) : Vector3(arr[0], arr[1], arr[2]) {}
		Vector3(const Vector4& v) : Vector4(v){}
		Vector3(const Vector3& ot) : Vector4(ot) {}
		~Vector3(){}

		__forceinline const bool operator == (const Vector3& rhs) const { return (_mm_movemask_ps(_mm_cmpeq_ps(m128, rhs.m128)) & 0x7) == 0x7; }
		__forceinline const bool operator == (const float& rhs) const { return *this == Vector3(rhs); }
		__forceinline const bool operator != (const Vector3& rhs) const { return (_mm_movemask_ps(_mm_cmpeq_ps(m128, rhs.m128)) & 0x7) != 0x7; }
		__forceinline const bool operator != (const float& rhs) const { return *this != Vector3(rhs); }

		__forceinline Vector3 operator + () const { return Vector4::operator+(); }
		__forceinline Vector3 operator - () const { return Vector4::operator-(); }
		__forceinline Vector3 operator + (const Vector3& ot) const { return Vector4::operator+(ot); }
		__forceinline Vector3 operator - (const Vector3& ot) const { return Vector4::operator-(ot); }
		__forceinline Vector4 operator * (const Vector4& ot) const { return Vector4::operator*(ot); }
		__forceinline Vector3 operator * (float m) const { return Vector4::operator*(m); }
		__forceinline Vector4 operator / (const Vector4& ot) const { return Vector4::operator/(ot); }
		__forceinline Vector3 operator / (float d) const { return Vector4::operator/(d); }
		__forceinline const Vector3& operator += (const Vector3& ot){ Vector4::operator+=(ot); return *this; }
		__forceinline const Vector3& operator -= (const Vector3& ot){ Vector4::operator-=(ot); return *this; }
		__forceinline const Vector3& operator *= (float m){ Vector4::operator*=(m); return *this; }
		__forceinline const Vector3& operator /= (float d){ Vector4::operator/=(d); return *this; }

		__forceinline float LengthSqr() const;
		__forceinline float Length() const;
		__forceinline Vector3& Normalize();
	}; 

	__forceinline Vector3 Normalized(const Vector3& v) { return v * Math::Rsqrt(v.LengthSqr()); }
	__forceinline Vector3 operator * (float r, const Vector3& v) { return v * r; }
	__forceinline std::ostream& operator << (std::ostream& os, const Vector3& v) {
		return os << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
	}

	__forceinline float Dot(const Vector3& u, const Vector3& v){
		return _mm_cvtss_f32(_mm_dp_ps(u, v, 0x71));	// dp[0,1,2] -> r[0]
	}
	__forceinline float AbsDot(const Vector3& u, const Vector3& v){
		return Math::Abs(Dot(u, v));
	}
	__forceinline Vector3 Cross(const Vector3& u, const Vector3& v){
		//return Vector3(
		//	u.y * v.z - u.z * v.y,
		//	u.z * v.x - u.x * v.z,
		//	u.x * v.y - u.y * v.x);
		return Shuffle<1, 2, 0, 3>(v * Shuffle<1, 2, 0, 3>(u) - u * Shuffle<1, 2, 0, 3>(v));
	}

	__forceinline float Vector3::LengthSqr() const { return Dot(*this, *this); }
	__forceinline float Vector3::Length() const { return Math::Sqrt(LengthSqr()); }
	__forceinline Vector3& Vector3::Normalize() { (*this) *= Math::Rsqrt(LengthSqr()); return *this; }
}