#pragma once

#include "Util.h"
#include "MathUtil.h"
#include "Vector.h"

namespace TX{
	// 4x4 row-major matrix using right-handed coordinate system
	class Matrix4x4 : public Align16 {
	public:
		static const Matrix4x4 IDENTITY;
	public: 
		// Constructs an identity matrix.
		Matrix4x4() :Matrix4x4(IDENTITY){}
		Matrix4x4(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3){ 
			row[0] = r0; 
			row[1] = r1; 
			row[2] = r2; 
			row[3] = r3;
		}
		Matrix4x4(const float matrix[4][4]){
			row[0] = Vector4(matrix[0]); 
			row[1] = Vector4(matrix[1]); 
			row[2] = Vector4(matrix[2]); 
			row[3] = Vector4(matrix[3]);
		}
		Matrix4x4(const Matrix4x4& ot) :Matrix4x4(ot[0], ot[1], ot[2], ot[3]){}
		Matrix4x4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: Matrix4x4(
			Vector4(m00, m01, m02, m03),
			Vector4(m10, m11, m12, m13),
			Vector4(m20, m21, m22, m23),
			Vector4(m30, m31, m32, m33)){}
		~Matrix4x4(){}

		inline Matrix4x4& operator = (const Matrix4x4& ot){
			memcpy_s(row, sizeof(row), ot.row, 4 * sizeof(Vector4));
			return *this;
		}
		inline bool operator == (const Matrix4x4& ot){
			return row[0] == ot[0] && row[1] == ot[1] && row[2] == ot[2] && row[3] == ot[3];
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
			//for (int i = 0; i < 4; i++)
			//	for (int j = 0; j < 4; j++)
			//		result.m[i][j] =
			//		m[i][0] * ot.m[0][j] +
			//		m[i][1] * ot.m[1][j] +
			//		m[i][2] * ot.m[2][j] +
			//		m[i][3] * ot.m[3][j];
			return Matrix4x4(
				ot[0]	* Shuffle<0, 0, 0, 0>(row[0])
				+ ot[1] * Shuffle<1, 1, 1, 1>(row[0])
				+ ot[2] * Shuffle<2, 2, 2, 2>(row[0])
				+ ot[3] * Shuffle<3, 3, 3, 3>(row[0]),
				ot[0]	* Shuffle<0, 0, 0, 0>(row[1])
				+ ot[1] * Shuffle<1, 1, 1, 1>(row[1])
				+ ot[2] * Shuffle<2, 2, 2, 2>(row[1])
				+ ot[3] * Shuffle<3, 3, 3, 3>(row[1]),
				ot[0]	* Shuffle<0, 0, 0, 0>(row[2])
				+ ot[1] * Shuffle<1, 1, 1, 1>(row[2])
				+ ot[2] * Shuffle<2, 2, 2, 2>(row[2])
				+ ot[3] * Shuffle<3, 3, 3, 3>(row[2]),
				ot[0]	* Shuffle<0, 0, 0, 0>(row[3])
				+ ot[1] * Shuffle<1, 1, 1, 1>(row[3])
				+ ot[2] * Shuffle<2, 2, 2, 2>(row[3])
				+ ot[3] * Shuffle<3, 3, 3, 3>(row[3]));
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

		inline const Vector4& operator[](int rowi) const { return row[rowi]; }
		inline Vector4& operator[](int rowi){ return row[rowi]; }

		inline Matrix4x4 Transpose() const{
			Matrix4x4 res(*this);
			_MM_TRANSPOSE4_PS(res.row[0], res.row[1], res.row[2], res.row[3]);
			return res;
		}
		inline Matrix4x4 Inverse() const{
			return InverseGeneral();
		}
		inline Matrix4x4 InverseGeneral() const {
			//float dst[4][4];
			//const float *src = *m;
			// Intel AP-928
			Vector4 minor0, minor1, minor2, minor3;
			Vector4 row0, row1, row2, row3;
			Vector4 det, tmp1;

			// Matrix transposition (not transpose)
			//tmp1 = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(src)), (__m64*)(src + 4));
			//row1 = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(src + 8)), (__m64*)(src + 12));
			//row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
			//row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
			//tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src + 2)), (__m64*)(src + 6));
			//row3 = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(src + 10)), (__m64*)(src + 14));
			//row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
			//row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);
			tmp1 = UnpackLow(row[0], row[1]);
			row1 = UnpackLow(row[2], row[3]);
			row0 = Shuffle<0, 1, 0, 1>(tmp1, row1);
			row1 = Shuffle<2, 3, 2, 3>(row1, tmp1);
			tmp1 = UnpackHigh(row[0], row[1]);
			row3 = UnpackHigh(row[2], row[3]);
			row2 = Shuffle<0, 1, 0, 1>(tmp1, row3);
			row3 = Shuffle<2, 3, 2, 3>(row3, tmp1);
			
			// Cofactors calculation. 
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

			// Evaluation of determinant and its reciprocal value. 
			// 1/det is evaluated using a fast rcpps command with subsequent approximation using the Newton-Raphson algorithm.
			det = _mm_mul_ps(row0, minor0);
			det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
			det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
			tmp1 = _mm_rcp_ss(det);
			det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
			det = _mm_shuffle_ps(det, det, 0x00);						// 

			// Multiplication of cofactors by 1/det.
			minor0 = _mm_mul_ps(det, minor0);
			//_mm_storel_pi((__m64*)(dst), minor0);
			//_mm_storeh_pi((__m64*)(dst + 2), minor0);
			minor1 = _mm_mul_ps(det, minor1);
			//_mm_storel_pi((__m64*)(dst + 4), minor1);
			//_mm_storeh_pi((__m64*)(dst + 6), minor1);
			minor2 = _mm_mul_ps(det, minor2);
			//_mm_storel_pi((__m64*)(dst + 8), minor2);
			//_mm_storeh_pi((__m64*)(dst + 10), minor2);
			minor3 = _mm_mul_ps(det, minor3);
			//_mm_storel_pi((__m64*)(dst + 12), minor3);
			//_mm_storeh_pi((__m64*)(dst + 14), minor3);				// 

			return Matrix4x4(minor0, minor1, minor2, minor3);
		}

		// Transforms a point
		static inline Vector3 TPoint(const Matrix4x4& m, const Vector3& p){
			// apply translation
			//Matrix4x4 t = m.Transpose();
			//return Shuffle<0, 0, 0, 3>(p) * t[0]
			//	+ Shuffle<1, 1, 1, 3>(p) * t[1]
			//	+ Shuffle<2, 2, 2, 3>(p) * t[2]
			//	+ t[3];
			Vector4 p4 = p;
			p4.w = 1.f;
			return Vector3(Dot4(p4, m.row[0]), Dot4(p4, m.row[1]), Dot4(p4, m.row[2]));
		}
		// Transforms a vector
		static inline Vector3 TVector(const Matrix4x4& m, const Vector3& v){
			// ignore translation
			return Vector3(Dot(v, m.row[0]), Dot(v, m.row[1]), Dot(v, m.row[2]));
		}
		// Transforms a normal vector with an already inverted matrix, the result is not normalized
		static inline Vector3 TNormal(const Matrix4x4& m_inv, const Vector3& n){
			// multiply the transpose
			return Shuffle<0, 0, 0, 3>(n) * m_inv.row[0]
				+ Shuffle<1, 1, 1, 3>(n) * m_inv.row[1]
				+ Shuffle<2, 2, 2, 3>(n) * m_inv.row[2];
		}

		static Matrix4x4 Translate(const Vector3& v);
		static Matrix4x4 Translate(float x, float y, float z);

		static Matrix4x4 Rotate(const Vector3& angle);
		static Matrix4x4 Rotate(float x, float y, float z);
		static Matrix4x4 Rotate(float angle, const Vector3& axis);
		static Matrix4x4 LookAt(const Vector3& pEye, const Vector3& pTarget, const Vector3& up);

		static Matrix4x4 Scale(const Vector3& s);
		static Matrix4x4 Scale(float x, float y, float z);

		// Orthographic projection matrix, size is a similar term to fov
		static Matrix4x4 Orthographic(float ratio, float size, float near, float far);
		// Perspective projection matrix, fov is vertical
		static Matrix4x4 Perspective(float ratio, float fov, float near, float far);
		// Viewport matrix
		static Matrix4x4 Viewport(float resx, float resy);
	private:
		Vector4 row[4];
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