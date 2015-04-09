#pragma once

#include "vector.h"

namespace Cruisky{
	// 4x4 row-major matrix using right-handed coordinate system
	class Matrix4x4{
	public: 
		static const Matrix4x4 IDENTITY;
		float m[4][4];
		Matrix4x4();
		Matrix4x4(float matrix[4][4]);
		Matrix4x4(const Matrix4x4& ot);
		Matrix4x4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);
		~Matrix4x4(){}

		Matrix4x4 Transpose();
		Matrix4x4 Inverse();

		// Construct a translation matrix
		static Matrix4x4 Translate(const Vector3& v);
		// Construct a rotation matrix along arbitray axis
		static Matrix4x4 Rotate(float angle, const Vector3& axis);
		// Construct a yaw-pitch-row rotation matrix 
		static Matrix4x4 Rotate(float x_angle, float y_angle, float z_angle);
		// Construct a yaw-pitch-row rotation matrix 
		static inline Matrix4x4 Rotate(const Vector3& angle);
		// Construct a scale matrix
		static Matrix4x4 Scale(const Vector3& s);
		// Construct a look-at matrix
		static Matrix4x4 LookAt(const Vector3& pt, const Vector3& dir, const Vector3& up);

		// Transform a point
		static inline Vector3 TPoint(const Matrix4x4& m, const Vector3& v);
		// Transform a vector
		static inline Vector3 TVector(const Matrix4x4& m, const Vector3& v);
		// Transform a normal vector with an already inverted matrix
		static inline Vector3 TNormal(const Matrix4x4& m_inv, const Vector3& n);

	private:
		inline bool IsAffine();
		inline Matrix4x4 InverseAffine();
		inline Matrix4x4 InverseGeneral();
	};


	Matrix4x4 operator * (const Matrix4x4& a, const Matrix4x4& b){
		Matrix4x4 result;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				result.m[i][j] =
				a.m[i][0] * b.m[0][j] +
				a.m[i][1] * b.m[1][j] +
				a.m[i][2] * b.m[2][j] +
				a.m[i][3] * b.m[3][j];
		return result;
	}
}