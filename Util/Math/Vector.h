#pragma once

#include "Util.h"
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
}