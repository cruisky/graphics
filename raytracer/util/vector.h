#pragma once

#include "math.h"
#include "dependency.h"

namespace Cruisky {
	class Vector3{
	public:
		static const Vector3 ZERO;
		static const Vector3 ONE;
	public:
		float x, y, z;
	public:
		__forceinline Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
		__forceinline Vector3(float x, float y, float z) : x(x), y(y), z(z) { Valid(); }
		__forceinline Vector3(const Vector3& ot) : x(ot.x), y(ot.y), z(ot.z) { Valid(); }
		~Vector3(){}

		__forceinline Vector3& operator = (const Vector3& ot){ x = ot.x; y = ot.y; z = ot.z; return *this; }

		__forceinline Vector3 operator + () const { return Vector3(+x, +y, +z); }
		__forceinline Vector3 operator - () const { return Vector3(-x, -y, -z); }

		__forceinline Vector3 operator + (const Vector3& ot) const { return Vector3(x + ot.x, y + ot.y, z + ot.z); }
		__forceinline Vector3 operator - (const Vector3& ot) const { return Vector3(x - ot.x, y - ot.y, z - ot.z); }
		__forceinline Vector3 operator * (float r) const { return Vector3(x * r, y * r, z * r); }
		__forceinline Vector3 operator / (float d) const { return Vector3(x / d, y / d, z / d); }

		__forceinline const Vector3& operator += (const Vector3& ot){ x += ot.x; y += ot.y; z += ot.z; Valid(); return *this; }
		__forceinline const Vector3& operator -= (const Vector3& ot){ x -= ot.x; y -= ot.y; z -= ot.z; Valid(); return *this; }
		__forceinline const Vector3& operator *= (float s){ x *= s; y *= s; z *= s; Valid(); return *this; }
		__forceinline const Vector3& operator /= (float d){ x /= d; y /= d; z /= d; Valid(); return *this; }

		__forceinline bool operator == (const Vector3 ot) const { return x == ot.x && y == ot.y && z == ot.z; }
		__forceinline bool operator != (const Vector3 ot) const { return x != ot.x || y != ot.y || z != ot.z; }
		
		__forceinline Vector3& Normalize();
		__forceinline void Valid() const { assert(!Math::IsNAN(x));	assert(!Math::IsNAN(y)); assert(!Math::IsNAN(z));
		}
	}; 

	__forceinline Vector3 operator * (float r, const Vector3& v) { return Vector3(v.x * r, v.y * r, v.z * r); }
	__forceinline std::ostream& operator << (std::ostream& os, const Vector3& v) { 
		return os << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
	}

	__forceinline float Dot(const Vector3& u, const Vector3& v){ return u.x * v.x + u.y * v.y + u.z * v.z; }
	__forceinline Vector3 Cross(const Vector3& u, const Vector3& v){
		return Vector3(
			u.y * v.z - u.z * v.y,
			u.z * v.x - u.x * v.z,
			u.x * v.y - u.y * v.x);
	}
	__forceinline float LengthSqr(const Vector3& v) { return Dot(v, v); }
	__forceinline float Length(const Vector3& v) { return Math::Sqrt(LengthSqr(v)); }
	__forceinline Vector3 Normalized(const Vector3& v) { return v * Math::Rsqrt(LengthSqr(v)); }

	__forceinline Vector3& Vector3::Normalize() { (*this) *= Math::Rsqrt(LengthSqr(*this)); return *this; }
}