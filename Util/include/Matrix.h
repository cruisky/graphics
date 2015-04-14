#pragma once

#include "fwddecl.h"
#include "MathUtil.h"
#include <iostream>
#include <cassert>

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

		Matrix4x4& operator = (const Matrix4x4& ot);

		Matrix4x4 operator * (const Matrix4x4& ot) const;
		const Matrix4x4& operator *= (const Matrix4x4 ot);

		float *operator[](int row){ return m[row]; }
		const float *operator[](int row) const { return m[row]; }

		//int &operator()(int row, int col);

		Matrix4x4 Transpose() const;
		Matrix4x4 Inverse() const;
		// Returns the inverse of a rotation matrix
		Matrix4x4 InverseRotation() const;

		static Matrix4x4 Translate(const Vector3& v);
		static Matrix4x4 Translate(float x, float y, float z);

		static Matrix4x4 Rotate(const Vector3& angle);
		static Matrix4x4 Rotate(float x, float y, float z);
		static Matrix4x4 Rotate(float angle, const Vector3& axis);
		static Matrix4x4 LookAt(const Vector3& pEye, const Vector3& pTarget, const Vector3& up);

		static Matrix4x4 Scale(const Vector3& s);
		static Matrix4x4 Scale(float x, float y, float z);

		// Orthographic projection matrix, size is a similar term to fov
		static Matrix4x4 Orthographic(float ratio, float size = 10.f, float near = 0.1f, float far = 1000.f);
		// Perspective projection matrix, fov is vertical
		static Matrix4x4 Perspective(float ratio, float fov = 90.f, float near = 0.1f, float far = 1000.f);
		// Viewport matrix
		static Matrix4x4 Viewport(float resx, float resy);

		// Transforms a point
		static Vector3 TPoint(const Matrix4x4& m, const Vector3& v);
		// Transforms a vector
		static Vector3 TVector(const Matrix4x4& m, const Vector3& v);
		// Transforms a normal vector with an already inverted matrix, the result is not normalized
		static Vector3 TNormal(const Matrix4x4& m_inv, const Vector3& n);
		
		bool IsAffine() const;
		Matrix4x4 InverseAffine() const;
		Matrix4x4 InverseGeneral() const;
	private:
		friend std::ostream& operator << (std::ostream& os, const Matrix4x4& M);
		
		// Apply the 3x3 part to the translation part
		Matrix4x4& InternalApplyTranslation();
		Matrix4x4& InternalTranspose();
		Matrix4x4& InternalInverse();

		float m[4][4];
	};

	inline Matrix4x4& Matrix4x4::operator = (const Matrix4x4& ot){
		memcpy(m, ot.m, 16 * sizeof(float));
		return *this;
	}

	inline Matrix4x4 Matrix4x4::operator * (const Matrix4x4& ot) const {
		Matrix4x4 result;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				result.m[i][j] =
				m[i][0] * ot.m[0][j] +
				m[i][1] * ot.m[1][j] +
				m[i][2] * ot.m[2][j] +
				m[i][3] * ot.m[3][j];
		return result;
	}

	inline const Matrix4x4& Matrix4x4::operator *= (const Matrix4x4 ot){
		float result[4][4];
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				result[i][j] =
				m[i][0] * ot.m[0][j] +
				m[i][1] * ot.m[1][j] +
				m[i][2] * ot.m[2][j] +
				m[i][3] * ot.m[3][j];
		memcpy(m, result, 16 * sizeof(float));
		return *this;
	}

	inline std::ostream& operator << (std::ostream& os, const Matrix4x4& m){
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++)
				os << m.m[i][j] << "\t";
			os << std::endl;
		}
		return os;
	}

	inline Matrix4x4& Matrix4x4::InternalApplyTranslation(){
		float v[3];
		v[0] = m[0][3] * m[0][0] + m[1][3] * m[0][1] + m[2][3] * m[0][2];
		v[1] = m[0][3] * m[1][0] + m[1][3] * m[1][1] + m[2][3] * m[1][2];
		v[2] = m[0][3] * m[2][0] + m[1][3] * m[2][1] + m[2][3] * m[2][2];
		m[0][3] = -v[0];
		m[1][3] = -v[1];
		m[2][3] = -v[2];
		return *this;
	}

	inline Matrix4x4& Matrix4x4::InternalTranspose(){
		float temp;
		temp = m[0][1]; m[0][1] = m[1][0]; m[1][0] = temp;
		temp = m[0][2]; m[0][2] = m[2][0]; m[2][0] = temp;
		temp = m[1][2]; m[1][2] = m[2][1]; m[2][1] = temp;
		return *this;
	}

	inline Matrix4x4& Matrix4x4::InternalInverse(){
		float cf[3][3];
		float det, det_inv;
		// find 3x3 matrix inverse
		cf[0][0] = m[1][1] * m[2][2] - m[2][1] * m[1][2];
		cf[0][1] = m[0][2] * m[2][0] - m[2][2] * m[0][1];
		cf[0][2] = m[0][1] * m[1][2] - m[1][1] * m[0][2];
		det = m[0][0] * cf[0][0] + m[0][1] * cf[0][1] + m[0][2] * cf[0][2];
		assert(Math::Abs(det) > Math::EPSILON);		// check if it's invertible
		cf[1][0] = m[2][0] * m[1][2] - m[1][0] * m[2][2];
		cf[1][1] = m[0][0] * m[2][2] - m[2][0] * m[0][2];
		cf[1][2] = m[1][0] * m[0][2] - m[0][0] * m[1][2];
		cf[2][0] = m[1][0] * m[2][1] - m[2][0] * m[1][1];
		cf[2][1] = m[2][0] * m[0][1] - m[0][0] * m[2][1];
		cf[2][2] = m[0][0] * m[1][1] - m[1][0] * m[0][1];
		det_inv = 1.f / det;
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++)
				m[i][j] *= cf[i][j] * det_inv;
		}
		return *this;
	}

	inline bool Matrix4x4::IsAffine() const {
		return m[3][0] == 0.f && m[3][1] == 0.f && m[3][2] == 0.f && m[3][3] == 1.f;
	}

	inline Matrix4x4 Matrix4x4::InverseAffine() const {
		Matrix4x4 result(*this);
		result.InternalInverse();
		return result.InternalApplyTranslation();
	}

	inline Matrix4x4 Matrix4x4::InverseGeneral() const {
		float res[4][4];
		const float *src = *m;
		float *dst = *res;
		// Intel AP-928
		__m128 minor0, minor1, minor2, minor3;
		__m128 row0, row1, row2, row3;
		__m128 det, tmp1;
		tmp1 = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(src)), (__m64*)(src + 4));
		row1 = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(src + 8)), (__m64*)(src + 12));
		row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
		row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
		tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src + 2)), (__m64*)(src + 6));
		row3 = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(src + 10)), (__m64*)(src + 14));
		row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
		row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);					//
		tmp1 = _mm_mul_ps(row2, row3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor0 = _mm_mul_ps(row1, tmp1);
		minor1 = _mm_mul_ps(row0, tmp1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
		minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
		minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);				//
		tmp1 = _mm_mul_ps(row1, row2);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
		minor3 = _mm_mul_ps(row0, tmp1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
		minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
		minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);				//
		tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		row2 = _mm_shuffle_ps(row2, row2, 0x4E);
		minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
		minor2 = _mm_mul_ps(row0, tmp1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
		minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
		minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);				//
		tmp1 = _mm_mul_ps(row0, row1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
		minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
		minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));		//
		tmp1 = _mm_mul_ps(row0, row3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
		minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
		minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));		//
		tmp1 = _mm_mul_ps(row0, row2);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
		minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
		minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);		//
		det = _mm_mul_ps(row0, minor0);
		det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
		det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
		tmp1 = _mm_rcp_ss(det);
		det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
		det = _mm_shuffle_ps(det, det, 0x00);
		minor0 = _mm_mul_ps(det, minor0);
		_mm_storel_pi((__m64*)(dst), minor0);
		_mm_storeh_pi((__m64*)(dst + 2), minor0);
		minor1 = _mm_mul_ps(det, minor1);
		_mm_storel_pi((__m64*)(dst + 4), minor1);
		_mm_storeh_pi((__m64*)(dst + 6), minor1);
		minor2 = _mm_mul_ps(det, minor2);
		_mm_storel_pi((__m64*)(dst + 8), minor2);
		_mm_storeh_pi((__m64*)(dst + 10), minor2);
		minor3 = _mm_mul_ps(det, minor3);
		_mm_storel_pi((__m64*)(dst + 12), minor3);
		_mm_storeh_pi((__m64*)(dst + 14), minor3);
		return Matrix4x4(res);
	}

}