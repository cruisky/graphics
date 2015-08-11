#pragma once

#include "Util.h"
#include "MathUtil.h"

namespace TX
{
	class Vector2 {
	public:
		union{
			struct { float x, y; };
			struct { float u, v; };
			float f[2];
		};
		static const Vector2 X;
		static const Vector2 Y;
		static const Vector2 Z;
		static const Vector2 UNIT[2];
		static const Vector2 ZERO;
		static const Vector2 ONE;
	public:
		__forceinline Vector2() : x(0.f), y(0.f){}
		__forceinline Vector2(const Vector2& ot) : x(ot.x), y(ot.y){}
		__forceinline Vector2(float v) : x(v), y(v){}
		__forceinline Vector2(float x, float y) : x(x), y(y){}
		__forceinline Vector2& operator = (const Vector2& ot){ x = ot.x; y = ot.y; return *this; }

		__forceinline const float& operator [] (const size_t i) const { return f[i]; }
		__forceinline       float& operator [] (const size_t i)       { return f[i]; }
		
		__forceinline bool operator == (const Vector2& ot) const { return x == ot.x && y == ot.y; }
		__forceinline bool operator != (const Vector2& ot) const { return x != ot.x || y != ot.y; }
		__forceinline bool operator > (const Vector2& ot) const { return x > ot.x && y > ot.y; }
		__forceinline bool operator >= (const Vector2& ot) const { return x >= ot.x && y >= ot.y; }
		__forceinline bool operator < (const Vector2& ot) const { return x < ot.x && y < ot.y; }
		__forceinline bool operator <= (const Vector2& ot) const { return x <= ot.x && y <= ot.y; }

		__forceinline Vector2 operator + () const { return Vector2(+x, +y); }
		__forceinline Vector2 operator - () const { return Vector2(-x, -y); }
		__forceinline Vector2 operator + (const Vector2& ot) const { return Vector2(x + ot.x, y + ot.y); }
		__forceinline Vector2 operator - (const Vector2& ot) const { return Vector2(x - ot.x, y - ot.y); }
		__forceinline Vector2 operator * (const Vector2& ot) const { return Vector2(x * ot.x, y * ot.y); }
		__forceinline Vector2 operator / (const Vector2& ot) const { return Vector2(x / ot.x, y / ot.y); }

		__forceinline const Vector2& operator += (const Vector2& ot) { x += ot.x; y += ot.y; return *this; }
		__forceinline const Vector2& operator -= (const Vector2& ot) { x -= ot.x; y -= ot.y; return *this; }
		__forceinline const Vector2& operator *= (const Vector2& ot) { x *= ot.x; y *= ot.y; return *this; }
		__forceinline const Vector2& operator /= (const Vector2& ot) { x /= ot.x; y /= ot.y; return *this; }
	
		__forceinline float LengthSqr() const;
		__forceinline float Length() const;
		__forceinline Vector2& Normalize();
	};
	__forceinline std::ostream& operator << (std::ostream& os, const Vector2& v)
	{
		return os << "(" << v.x << ", " << v.y << ")";
	}
	__forceinline float Dot(const Vector2& u, const Vector2& v){ return u.x * v.x + u.y * v.y; }
	__forceinline float AbsDot(const Vector2& u, const Vector2& v){ return Math::Abs(Dot(u, v)); }
	__forceinline Vector2 Normalized(const Vector2& v) { return v * Math::Rsqrt(v.LengthSqr()); }
	__forceinline float Vector2::LengthSqr() const { return Dot(*this, *this); }
	__forceinline float Vector2::Length() const { return Math::Sqrt(LengthSqr()); }
	__forceinline Vector2& Vector2::Normalize() { (*this) *= Math::Rsqrt(LengthSqr()); return *this; }


	namespace Math {
		inline auto Min(const Vector2& v1, const Vector2& v2) -> decltype(v1 + v2) { return Vector2(Math::Min(v1.x, v2.x), Math::Min(v1.y, v2.y)); }
		inline auto Max(const Vector2& v1, const Vector2& v2) -> decltype(v1 + v2) { return Vector2(Math::Max(v1.x, v2.x), Math::Max(v1.y, v2.y)); }
	}

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

		__forceinline Vector3 operator + (const Vector3& ot) const { return Vector3(x + ot.x, y + ot.y, z + ot.z); }
		__forceinline Vector3 operator - (const Vector3& ot) const { return Vector3(x - ot.x, y - ot.y, z - ot.z); }
		__forceinline Vector3 operator * (const Vector3& ot) const { return Vector3(x * ot.x, y * ot.y, z * ot.z); }
		__forceinline Vector3 operator * (float s) const { return Vector3(x * s, y * s, z * s); }
		__forceinline Vector3 operator / (const Vector3& ot) const { return Vector3(x / ot.x, y / ot.y, z / ot.z); }
		__forceinline Vector3 operator / (float d) const { return Vector3(x / d, y / d, z / d); }
		__forceinline const Vector3& operator += (const Vector3& ot) { x += ot.x; y += ot.y; z += ot.z; return *this; }
		__forceinline const Vector3& operator -= (const Vector3& ot) { x -= ot.x; y -= ot.y; z -= ot.z; return *this; }
		__forceinline const Vector3& operator *= (const Vector3& ot) { x *= ot.x; y *= ot.y; z *= ot.z; return *this; }
		__forceinline const Vector3& operator *= (float s) { x *= s; y *= s; z *= s; return *this; }
		__forceinline const Vector3& operator /= (const Vector3& ot) { x /= ot.x; y /= ot.y; z /= ot.z; return *this; }
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
		__forceinline Vector4(const float *arr) : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3]){}
		__forceinline Vector4(const Vector4& ot) : x(ot.x), y(ot.y), z(ot.z), w(ot.w){}

		__forceinline operator const float*() const { return f; }
		__forceinline operator float*()       { return f; }

		__forceinline bool operator == (const Vector4& ot) const { return x == ot.x && y == ot.y && z == ot.z && w == ot.w; }
		__forceinline bool operator != (const Vector4& ot) const { return x != ot.x || y != ot.y || z != ot.z || w != ot.w; }

		__forceinline Vector4& operator = (const Vector4& ot){ x = ot.x; y = ot.y; z = ot.z; w = ot.w; return *this; }
		__forceinline const Vector4 operator + () const { return Vector4(+x, +y, +z, +w); }
		__forceinline const Vector4 operator - () const { return Vector4(-x, -y, -z, -w); }

		__forceinline const Vector4 operator + (const Vector4& ot) const { return Vector4(x + ot.x, y + ot.y, z + ot.z, w + ot.w); }
		__forceinline const Vector4 operator - (const Vector4& ot) const { return Vector4(x - ot.x, y - ot.y, z - ot.z, w - ot.w); }
		__forceinline const Vector4 operator * (const Vector4& ot) const { return Vector4(x * ot.x, y * ot.y, z * ot.z, w * ot.w); }
		__forceinline const Vector4 operator * (float s) const { return Vector4(x * s, y * s, z * s, w * s); }
		__forceinline const Vector4 operator / (const Vector4& ot) const { return Vector4(x / ot.x, y / ot.y, z / ot.z, w / ot.w); }
		__forceinline const Vector4 operator / (float d) const { return Vector4(x / d, y / d, z / d, w / d); }
		__forceinline Vector4& operator += (const Vector4& ot) { x += ot.x; y += ot.y; z += ot.z; w += ot.w; return *this; }
		__forceinline Vector4& operator -= (const Vector4& ot) { x -= ot.x; y -= ot.y; z -= ot.z; w -= ot.w; return *this; }
		__forceinline Vector4& operator *= (const Vector4& ot) { x *= ot.x; y *= ot.y; z *= ot.z; w *= ot.w; return *this; }
		__forceinline Vector4& operator *= (float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
		__forceinline Vector4& operator /= (const Vector4& ot) { x /= ot.x; y /= ot.y; z /= ot.z; w /= ot.w; return *this; }
		__forceinline Vector4& operator /= (float d) { x /= d; y /= d; z /= d; z /= d; return *this; }

		__forceinline float LengthSqr() const;
		__forceinline float Length() const;
	};
	__forceinline std::ostream& operator << (std::ostream& os, const Vector4& v)
	{
		return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	}
	__forceinline Vector4 operator * (float s, const Vector4& v) { return v * s; }

	__forceinline float Dot(const Vector4& u, const Vector4& v){ return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w; }
	__forceinline float AbsDot(const Vector4& u, const Vector4& v){ return Math::Abs(Dot(u, v)); }
	__forceinline float Vector4::LengthSqr() const { return Dot(*this, *this); }
	__forceinline float Vector4::Length() const { return Math::Sqrt(LengthSqr()); }
}