#pragma once

#include "Util.h"
#include "Base.h"
#include "Vector.h"

namespace TX{
	class Matrix3x3 {
	public:
		static const Matrix3x3 IDENTITY;
	private:
		Vec3 row[3];
	public:
		Matrix3x3(){
			row[0] = Vec3::X;
			row[1] = Vec3::Y;
			row[2] = Vec3::Z;
		}
		Matrix3x3(const Vec3& r0, const Vec3& r1, const Vec3& r2){
			row[0] = r0;
			row[1] = r1;
			row[2] = r2;
		}
		Matrix3x3(const Matrix3x3& ot) : Matrix3x3(ot[0], ot[1], ot[2]){}
		Matrix3x3(
			float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22)
			: Matrix3x3(
			Vec3(m00, m01, m02),
			Vec3(m10, m11, m12),
			Vec3(m20, m21, m22)){}
		~Matrix3x3(){}

		inline Matrix3x3& operator = (const Matrix3x3& ot){
			row[0] = ot[0];
			row[1] = ot[1];
			row[2] = ot[2];
			return *this;
		}

		inline Matrix3x3 operator + (const Matrix3x3& ot) const{
			return Matrix3x3(
				row[0] + ot[0],
				row[1] + ot[1],
				row[2] + ot[2]);
		}
		inline Matrix3x3 operator - (const Matrix3x3& ot) const{
			return Matrix3x3(
				row[0] - ot[0],
				row[1] - ot[1],
				row[2] - ot[2]);
		}
		inline Matrix3x3 operator * (const Matrix3x3& ot) const {
			Matrix3x3 result;
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					result[i][j] =
					row[i][0] * ot[0][j] +
					row[i][1] * ot[1][j] +
					row[i][2] * ot[2][j];
			return result;
		}
		inline const Matrix3x3& operator += (const Matrix3x3& ot){
			row[0] += ot[0];
			row[1] += ot[1];
			row[2] += ot[2];
			return *this;
		}
		inline const Matrix3x3& operator -= (const Matrix3x3& ot){
			row[0] -= ot[0];
			row[1] -= ot[1];
			row[2] -= ot[2];
			return *this;
		}
		inline const Matrix3x3& operator *= (const Matrix3x3& ot) {
			Vec3 temp;
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 3; j++)
					temp[j] =
					row[i][0] * ot[0][j] +
					row[i][1] * ot[1][j] +
					row[i][2] * ot[2][j];
				row[i] = temp;
			}
			return *this;
		}

		inline const Vec3& operator[](int rowi) const { return row[rowi]; }
		inline Vec3& operator[](int rowi){ return row[rowi]; }
		inline operator float*(){ return &row[0][0]; }
		inline operator const float*() const { return &row[0][0]; }

		inline Matrix3x3 Transpose() const{
			return Matrix3x3(
				row[0][0], row[1][0], row[2][0],
				row[0][1], row[1][1], row[2][1],
				row[0][2], row[1][2], row[2][2]);
		}

		inline Matrix3x3 Inverse() const {
			//TODO
		}
	};

	inline std::ostream& operator << (std::ostream& os, const Matrix3x3& m){
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++)
				os << m[i][j] << "\t";
			os << std::endl;
		}
		return os;
	}


	// 4x4 row-major matrix using right-handed coordinate system
	class Matrix4x4 {
	public:
		static const Matrix4x4 IDENTITY;
	private:
		Vec4 row[4];
	public:
		Matrix4x4(){
			row[0] = Vec4::X;
			row[1] = Vec4::Y;
			row[2] = Vec4::Z;
			row[3] = Vec4::W;
		}
		Matrix4x4(const Vec4& r0, const Vec4& r1, const Vec4& r2, const Vec4& r3){
			row[0] = r0;
			row[1] = r1;
			row[2] = r2;
			row[3] = r3;
		}
		Matrix4x4(const Matrix4x4& ot) :Matrix4x4(ot[0], ot[1], ot[2], ot[3]){}
		Matrix4x4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: Matrix4x4(
			Vec4(m00, m01, m02, m03),
			Vec4(m10, m11, m12, m13),
			Vec4(m20, m21, m22, m23),
			Vec4(m30, m31, m32, m33)){}
		~Matrix4x4(){}

		inline Matrix4x4& operator = (const Matrix4x4& ot){
			row[0] = ot[0];
			row[1] = ot[1];
			row[2] = ot[2];
			row[3] = ot[3];
			return *this;
		}

		inline Matrix4x4 operator + (const Matrix4x4& ot) const{
			return Matrix4x4(
				row[0] + ot[0],
				row[1] + ot[1],
				row[2] + ot[2],
				row[3] + ot[3]);
		}
		inline Matrix4x4 operator - (const Matrix4x4& ot) const{
			return Matrix4x4(
				row[0] - ot[0],
				row[1] - ot[1],
				row[2] - ot[2],
				row[3] - ot[3]);
		}
		inline Matrix4x4 operator * (const Matrix4x4& ot) const {
			Matrix4x4 result;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					result[i][j] =
					row[i][0] * ot[0][j] +
					row[i][1] * ot[1][j] +
					row[i][2] * ot[2][j] +
					row[i][3] * ot[3][j];
			return result;
		}
		inline const Matrix4x4& operator += (const Matrix4x4& ot){
			row[0] += ot[0];
			row[1] += ot[1];
			row[2] += ot[2];
			row[3] += ot[3];
			return *this;
		}
		inline const Matrix4x4& operator -= (const Matrix4x4& ot){
			row[0] -= ot[0];
			row[1] -= ot[1];
			row[2] -= ot[2];
			row[3] -= ot[3];
			return *this;
		}
		inline const Matrix4x4& operator *= (const Matrix4x4& ot) {
			Vec4 temp;
			for (int i = 0; i < 4; i++){
				for (int j = 0; j < 4; j++)
					temp[j] =
					row[i][0] * ot[0][j] +
					row[i][1] * ot[1][j] +
					row[i][2] * ot[2][j] +
					row[i][3] * ot[3][j];
				row[i] = temp;
			}
			return *this;
		}

		inline const Vec4& operator[](int rowi) const { return row[rowi]; }
		inline Vec4& operator[](int rowi){ return row[rowi]; }
		inline operator float*(){ return &row[0][0]; }
		inline operator const float*() const { return &row[0][0]; }

		inline Matrix4x4 Transpose() const{
			return Matrix4x4(
				row[0][0], row[1][0], row[2][0], row[3][0],
				row[0][1], row[1][1], row[2][1], row[3][1],
				row[0][2], row[1][2], row[2][2], row[3][2],
				row[0][3], row[1][3], row[2][3], row[3][3]);
		}

		inline Matrix4x4 Inverse() const {
			Matrix4x4 result;
			const float *src = (const float *)*row;
			float *dst = (float *)result[0];
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
			return result;
		}

		// Transforms a point
		static inline Vec3 TPoint(const Matrix4x4& m, const Vec3& p){
			// apply translation
			return Vec3(
				p.x * m[0][0] + p.y * m[0][1] + p.z * m[0][2] + m[0][3],
				p.x * m[1][0] + p.y * m[1][1] + p.z * m[1][2] + m[1][3],
				p.x * m[2][0] + p.y * m[2][1] + p.z * m[2][2] + m[2][3]);
		}
		// Transforms a vector
		static inline Vec3 TVec(const Matrix4x4& m, const Vec3& v){
			// ignore translation
			return Vec3(
				v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2],
				v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2],
				v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2]);
		}
		// Transforms a normal vector with an already inverted matrix, the result is not normalized
		static inline Vec3 TNormal(const Matrix4x4& m_inv, const Vec3& n){
			// multiply the transpose
			return Vec3(
				n.x * m_inv[0][0] + n.y * m_inv[1][0] + n.z * m_inv[2][0],
				n.x * m_inv[0][1] + n.y * m_inv[1][1] + n.z * m_inv[2][1],
				n.x * m_inv[0][2] + n.y * m_inv[1][2] + n.z * m_inv[2][2]);
		}

		static Matrix4x4 Translate(const Vec3& v);
		static Matrix4x4 Translate(float x, float y, float z);

		static Matrix4x4 Rotate(const Vec3& angle);
		static Matrix4x4 Rotate(float x, float y, float z);
		static Matrix4x4 Rotate(float angle, const Vec3& axis);
		static Matrix4x4 LookAt(const Vec3& pEye, const Vec3& pTarget, const Vec3& up);

		static Matrix4x4 Scale(const Vec3& s);
		static Matrix4x4 Scale(float x, float y, float z);

		// Orthographic projection matrix, size is a similar term to fov
		static Matrix4x4 Orthographic(float ratio, float size, float near, float far);
		// Perspective projection matrix, fov is vertical
		static Matrix4x4 Perspective(float ratio, float fov, float near, float far);
		// Viewport matrix
		static Matrix4x4 Viewport(float resx, float resy);
	};

	inline std::ostream& operator << (std::ostream& os, const Matrix4x4& m){
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++)
				os << m[i][j] << "\t";
			os << std::endl;
		}
		return os;
	}

	inline bool SolveLinearSystem2x2(const float A[2][2], const float B[2], float *x0, float *x1){
		float det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
		if (Math::Abs(det) < Math::EPSILON)
			return false;
		*x0 = (A[1][1] * B[0] - A[0][1] * B[1]) / det;
		*x1 = (A[0][0] * B[1] - A[1][0] * B[0]) / det;
		return !(Math::IsNAN(*x0) || Math::IsNAN(*x1));
	}
}
