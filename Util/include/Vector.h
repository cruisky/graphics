#pragma once

#include "../Util.h"
#include "MathUtil.h"

namespace TX {
	class Vector3{
	public:
		static const Vector3 ZERO;
		static const Vector3 ONE;
		static const Vector3 X;
		static const Vector3 Y;
		static const Vector3 Z;
	public:
		float x, y, z;
	public:
		Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) { Validate(); }
		Vector3(float arr[3]) : x(arr[0]), y(arr[1]), z(arr[2]) { Validate(); }
		Vector3(const Vector3& ot) : x(ot.x), y(ot.y), z(ot.z) { Validate(); }
		~Vector3(){}

		inline Vector3& operator = (const Vector3& ot){ x = ot.x; y = ot.y; z = ot.z; return *this; }

		inline Vector3 operator + () const { return Vector3(+x, +y, +z); }
		inline Vector3 operator - () const { return Vector3(-x, -y, -z); }

		inline Vector3 operator + (const Vector3& ot) const { return Vector3(x + ot.x, y + ot.y, z + ot.z); }
		inline Vector3 operator - (const Vector3& ot) const { return Vector3(x - ot.x, y - ot.y, z - ot.z); }
		inline Vector3 operator * (float r) const { return Vector3(x * r, y * r, z * r); }
		inline Vector3 operator / (float d) const { return Vector3(x / d, y / d, z / d); }

		inline const Vector3& operator += (const Vector3& ot){ x += ot.x; y += ot.y; z += ot.z; Validate(); return *this; }
		inline const Vector3& operator -= (const Vector3& ot){ x -= ot.x; y -= ot.y; z -= ot.z; Validate(); return *this; }
		inline const Vector3& operator *= (float s){ x *= s; y *= s; z *= s; Validate(); return *this; }
		inline const Vector3& operator /= (float d){ x /= d; y /= d; z /= d; Validate(); return *this; }

		inline bool operator == (const Vector3 ot) const { return x == ot.x && y == ot.y && z == ot.z; }
		inline bool operator != (const Vector3 ot) const { return x != ot.x || y != ot.y || z != ot.z; }

		inline float LengthSqr() const;
		inline float Length() const;
		inline Vector3& Normalize();

		inline void Validate() const {
			assert(!Math::IsNAN(x));	assert(!Math::IsNAN(y)); assert(!Math::IsNAN(z));
		}
	};

	inline Vector3 Normalized(const Vector3& v) { return v * Math::Rsqrt(v.LengthSqr()); }
	inline Vector3 operator * (float r, const Vector3& v) { return Vector3(v.x * r, v.y * r, v.z * r); }
	inline std::ostream& operator << (std::ostream& os, const Vector3& v) {
		return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	}

	inline float Dot(const Vector3& u, const Vector3& v){ return u.x * v.x + u.y * v.y + u.z * v.z; }
	inline float AbsDot(const Vector3& u, const Vector3& v){ return fabsf(u.x * v.x + u.y * v.y + u.z * v.z); }
	inline Vector3 Cross(const Vector3& u, const Vector3& v){
		return Vector3(
			u.y * v.z - u.z * v.y,
			u.z * v.x - u.x * v.z,
			u.x * v.y - u.y * v.x);
	}

	inline float Vector3::LengthSqr() const { return Dot(*this, *this); }
	inline float Vector3::Length() const { return Math::Sqrt(LengthSqr()); }
	inline Vector3& Vector3::Normalize() { (*this) *= Math::Rsqrt(LengthSqr()); return *this; }


	class Vector4{
	public:
		static const Vector4 ZERO;
		static const Vector4 ONE;
		static const Vector4 X;
		static const Vector4 Y;
		static const Vector4 Z;
		static const Vector4 W;
	public:
		Vector4() :v(_mm_setzero_ps()){}
		Vector4(__m128 v) :v(v){}
		Vector4(float x, float y, float z, float w){ v = _mm_set_ps(x, y, z, w); }
		Vector4(const float vec[4]) :v(_mm_load_ps(vec)){}
		Vector4(const Vector4& ot) :v(ot.v){}
		~Vector4(){}

		inline Vector4& operator = (const Vector4& ot){ v = ot.v; return *this; }

		inline Vector4 operator + () const { return Vector4(v); }
		inline Vector4 operator - () const { return Vector4(_mm_xor_ps(v, _mm_set1_ps(-0.0))); }

		inline Vector4 operator + (const Vector4& ot) const { return Vector4(_mm_add_ps(v, ot.v)); }
		inline Vector4 operator - (const Vector4& ot) const { return Vector4(_mm_sub_ps(v, ot.v)); }
		inline Vector4 operator * (float r) const { return Vector4(_mm_mul_ps(v, _mm_set1_ps(r))); }
		inline Vector4 operator / (float d) const { return Vector4(_mm_mul_ps(v, _mm_set1_ps(1.f/d))); }

		inline const Vector4& operator += (const Vector4& ot){ v = _mm_add_ps(v, ot.v); return *this; }
		inline const Vector4& operator -= (const Vector4& ot){ v = _mm_sub_ps(v, ot.v); return *this; }
		inline const Vector4& operator *= (float s){ v = _mm_mul_ps(v, _mm_set1_ps(s)); return *this; }
		inline const Vector4& operator /= (float d){ v = _mm_mul_ps(v, _mm_set1_ps(1.f / d)); return *this; }

		inline bool operator == (const Vector4 ot) const { return _mm_movemask_ps(_mm_cmpeq_ps(v, ot.v)) == 0xffff; }
		inline bool operator != (const Vector4 ot) const { return _mm_movemask_ps(_mm_cmpeq_ps(v, ot.v)) != 0xffff; }
	private:
		__m128 v;
	};
}
