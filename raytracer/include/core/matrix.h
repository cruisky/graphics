#pragma once

#include "fwddecl.h"
#include <iostream>

namespace Cruisky{
	// 4x4 row-major matrix using right-handed coordinate system
	class Matrix4x4{
	public: 
		static const Matrix4x4 IDENTITY;

		// Constructs an identity matrix.
		Matrix4x4();
		Matrix4x4(float matrix[4][4]);
		Matrix4x4(const Matrix4x4& ot);
		Matrix4x4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);
		~Matrix4x4(){}

		inline Matrix4x4 operator * (const Matrix4x4& ot) const;
		inline const Matrix4x4& operator *= (const Matrix4x4 ot);

		Matrix4x4 Transpose();
		Matrix4x4 Inverse();

		// Constructs a translation matrix
		static Matrix4x4 Translate(const Vector3& v);
		// Constructs a rotation matrix along arbitray axis
		static Matrix4x4 Rotate(float angle, const Vector3& axis);
		// Constructs a yaw-pitch-row rotation matrix 
		static Matrix4x4 Rotate(float x_angle, float y_angle, float z_angle);
		// Constructs a yaw-pitch-row rotation matrix 
		static Matrix4x4 Rotate(const Vector3& angle);
		// Constructs a scale matrix
		static Matrix4x4 Scale(const Vector3& s);
		// Constructs a look-at matrix
		static Matrix4x4 LookAt(const Vector3& pt, const Vector3& dir, const Vector3& up);

		// Transforms a point
		static Vector3 TPoint(const Matrix4x4& m, const Vector3& v);
		// Transforms a vector
		static Vector3 TVector(const Matrix4x4& m, const Vector3& v);
		// Transforms a normal vector with an already inverted matrix, the result is not normalized
		static Vector3 TNormal(const Matrix4x4& m_inv, const Vector3& n);
		
	private:
		friend inline std::ostream& operator << (std::ostream& os, const Matrix4x4& M);

		inline bool IsAffine();
		inline Matrix4x4 InverseAffine();
		inline Matrix4x4 InverseGeneral();

		float m[4][4];
	};
}