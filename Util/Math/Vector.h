#pragma once

#include "Util.h"
#include "MathUtil.h"

namespace TX
{
	class Vector3 {
	public:
		union{
			struct{ float x, y, z; };
			float f[3];
		};
		static const Vector3 X;
		static const Vector3 Y;
		static const Vector3 Z;
		static const Vector3 UNIT[3];
		static const Vector3 ZERO;
		static const Vector3 ONE;
		static const Vector3 PI;
		static const Vector3 QTR_PI;
		static const Vector3 HALF_PI;
		static const Vector3 TWO_PI;
		static const Vector3 FOUR_PI;
		static const Vector3 PI_RCP;
		static const Vector3 HALF_PI_RCP;
		static const Vector3 TWO_PI_RCP;
		static const Vector3 FOUR_PI_RCP;
	public:
		__forceinline Vector3() : x(0.f), y(0.f), z(0.f){}
		__forceinline Vector3(float f) : x(f), y(f), z(f){}
		__forceinline Vector3(float x, float y, float z) : x(x), y(y), z(z){}
		__forceinline Vector3(const Vector3& ot) : x(ot.x), y(ot.y), z(ot.z){}

		__forceinline const float& operator [] (const size_t i) const { return f[i]; }
		__forceinline		float& operator [] (const size_t i)		  { return f[i]; }

		__forceinline bool operator == (const Vector3& ot) const { return x == ot.x && y == ot.y && z == ot.z; }
		__forceinline bool operator != (const Vector3& ot) const { return x != ot.x || y != ot.y || z != ot.z; }

		__forceinline Vector3& operator = (const Vector3& ot){ x = ot.x; y = ot.y; z = ot.z; return *this; }
		__forceinline Vector3 operator + () const { return Vector3(+x, +y, +z); }
		__forceinline Vector3 operator - () const { return Vector3(-x, -y, -z); }

		__forceinline Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
		__forceinline Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
		__forceinline Vector3 operator * (const Vector3& rhs) const { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z); }
		__forceinline Vector3 operator * (float s) const { return Vector3(x * s, y * s, z * s); }
		__forceinline Vector3 operator / (const Vector3& rhs) const { return Vector3(x / rhs.x, y / rhs.y, z / rhs.z); }
		__forceinline Vector3 operator / (float d) const { return Vector3(x / d, y / d, z / d); }
		__forceinline const Vector3& operator += (const Vector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
		__forceinline const Vector3& operator -= (const Vector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
		__forceinline const Vector3& operator *= (const Vector3& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
		__forceinline const Vector3& operator *= (float s) { x *= s; y *= s; z *= s; return *this; }
		__forceinline const Vector3& operator /= (const Vector3& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }
		__forceinline const Vector3& operator /= (float d) { x /= d; y /= d; z /= d; return *this; }

		__forceinline float LengthSqr() const;
		__forceinline float Length() const;
		__forceinline Vector3& Normalize();

	};
	__forceinline std::ostream& operator << (std::ostream& os, const Vector3& v)
	{
		return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	}
	__forceinline Vector3 operator * (float s, const Vector3& v) { return v * s; }

	__forceinline float Dot(const Vector3& u, const Vector3& v){ return u.x * v.x + u.y*v.y + u.z*v.z; }
	__forceinline float AbsDot(const Vector3& u, const Vector3& v){ return Math::Abs(Dot(u, v)); }
	__forceinline Vector3 Normalized(const Vector3& v) { return v * Math::Rsqrt(v.LengthSqr()); }
	__forceinline Vector3 Cross(const Vector3& u, const Vector3& v){
		return Vector3(
			u.y * v.z - u.z * v.y,
			u.z * v.x - u.x * v.z,
			u.x * v.y - u.y * v.x);
	}
	__forceinline float Vector3::LengthSqr() const { return Dot(*this, *this); }
	__forceinline float Vector3::Length() const { return Math::Sqrt(LengthSqr()); }
	__forceinline Vector3& Vector3::Normalize() { (*this) *= Math::Rsqrt(LengthSqr()); return *this; }


	class Vector4 {
	public:
		union{
			struct { float x, y, z, w; };
			float f[4];
		};
		static const Vector4 X;
		static const Vector4 Y;
		static const Vector4 Z;
		static const Vector4 W;
		static const Vector4 UNIT[4];
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
		__forceinline Vector4() : x(0.f), y(0.f), z(0.f), w(0.f){}
		__forceinline Vector4(float f) : x(f), y(f), z(f), w(f){}
		__forceinline Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){}
		__forceinline Vector4(const Vector4& ot) : x(ot.x), y(ot.y), z(ot.z), w(ot.w){}

		__forceinline operator const float*() const { return f; }
		__forceinline operator float*()       { return f; }

		__forceinline bool operator == (const Vector4& ot) const { return x == ot.x && y == ot.y && z == ot.z && w == ot.w; }
		__forceinline bool operator != (const Vector4& ot) const { return x != ot.x || y != ot.y || z != ot.z || w != ot.w; }

		__forceinline Vector4& operator = (const Vector4& ot){ x = ot.x; y = ot.y; z = ot.z; w = ot.w; return *this; }
		__forceinline const Vector4 operator + () const { return Vector4(+x, +y, +z, +w); }
		__forceinline const Vector4 operator - () const { return Vector4(-x, -y, -z, -w); }

		__forceinline const Vector4 operator + (const Vector4& rhs) const { return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
		__forceinline const Vector4 operator - (const Vector4& rhs) const { return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
		__forceinline const Vector4 operator * (const Vector4& rhs) const { return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w); }
		__forceinline const Vector4 operator * (float s) const { return Vector4(x * s, y * s, z * s, w * s); }
		__forceinline const Vector4 operator / (const Vector4& rhs) const { return Vector4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w); }
		__forceinline const Vector4 operator / (float d) const { return Vector4(x / d, y / d, z / d, w / d); }
		__forceinline Vector4& operator += (const Vector4& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
		__forceinline Vector4& operator -= (const Vector4& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
		__forceinline Vector4& operator *= (const Vector4& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this; }
		__forceinline Vector4& operator *= (float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
		__forceinline Vector4& operator /= (const Vector4& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w; return *this; }
		__forceinline Vector4& operator /= (float d) { x /= d; y /= d; z /= d; z /= d; return *this; }

	};
	__forceinline std::ostream& operator << (std::ostream& os, const Vector4& v)
	{
		return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	}
	__forceinline Vector4 operator * (float s, const Vector4& v) { return v * s; }
}