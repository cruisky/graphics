#pragma once

#include "core.h"
#include "math.h"

namespace Core {
	namespace Math {

		class Vector3{
		public:
			float x, y, z;
		public:
			__forceinline Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
			__forceinline Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
			__forceinline Vector3(const Vector3& ot) : x(ot.x), y(ot.y), z(ot.z) {}
			~Vector3(){}

			__forceinline Vector3& operator = (const Vector3& ot){
				x = ot.x; y = ot.y; z = ot.z;
			}

			__forceinline Vector3 operator + () const { return Vector3(+x, +y, +z); }
			__forceinline Vector3 operator - () const { return Vector3(-x, -y, -z); }

			__forceinline Vector3 operator + (const Vector3& ot) const { return Vector3(x + ot.x, y + ot.y, z + ot.z); }
			__forceinline Vector3 operator - (const Vector3& ot) const { return Vector3(x - ot.x, y - ot.y, z - ot.z); }
			__forceinline Vector3 operator * (float r) const { return Vector3(x * r, y * r, z * r); }
			__forceinline Vector3 operator / (float d) const { return Vector3(x / d, y / d, z / d); }

			__forceinline const Vector3& operator += (const Vector3& ot){ x += ot.x; y += ot.y; z += ot.z; return *this; }
			__forceinline const Vector3& operator -= (const Vector3& ot){ x -= ot.x; y -= ot.y; z -= ot.z; return *this; }
			__forceinline const Vector3& operator *= (float s){ x *= s; y *= s; z *= s; return *this; }
			__forceinline const Vector3& operator /= (float d){ x /= d; y /= d; z /= d; return *this; }

			__forceinline Vector3& Normalize();
		}; 

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
}