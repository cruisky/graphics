#include "core/matrix.h"
#include "core/vector.h"
#include "core/ray.h"
#include <iostream>
#include <string>

namespace Cruisky{
	const Matrix4x4 Matrix4x4::IDENTITY = Matrix4x4();

	Matrix4x4::Matrix4x4(){
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
		m[0][1] = m[0][2] = m[0][3] = m[1][0] =
			m[1][2] = m[1][3] = m[2][0] = m[2][1] =
			m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.f;
	}

	Matrix4x4::Matrix4x4(float matrix[4][4]){
		memcpy(m, matrix, 16 * sizeof(float));
	}

	Matrix4x4::Matrix4x4(const Matrix4x4& ot){
		memcpy(m, ot.m, 16 * sizeof(float));
	}

	Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33){
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}

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
	}

	Matrix4x4 Matrix4x4::Transpose(){
		return Matrix4x4(
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]
			);
	}
		
	Matrix4x4 Matrix4x4::Inverse(){
		if (IsAffine())
			return InverseAffine();
		else
			return InverseGeneral();
	}

	Matrix4x4 Matrix4x4::Translate(const Vector3& v){
		return Matrix4x4(
			1.f, 0.f, 0.f, v.x,
			0.f, 1.f, 0.f, v.y,
			0.f, 0.f, 1.f, v.z,
			0.f, 0.f, 0.f, 1.f
			);
	}
	Matrix4x4 Matrix4x4::Rotate(float angle, const Vector3& axis){
		Vector3 a(axis);
		a.Normalize();
		angle = Math::ToRad(angle);
		float c = Math::Cos(angle), s = Math::Sin(angle), c1 = 1 - c;
		return Matrix4x4(
			a.x*a.x*c1+c,		a.x*a.y*c1-a.z*s,	a.x*a.z*c1+a.y*s,	0.f,
			a.x*a.y*c1+a.z*s,	a.y*a.y*c1+c,		a.y*a.z*c1-a.x*s,	0.f,
			a.x*a.z*c1-a.y*s,	a.y*a.z*c1+a.x*s,	a.z*a.z*c1+c,		0.f,
			0.f,				0.f,				0.f,				1.f);
	}

	Matrix4x4 Matrix4x4::Rotate(float x_angle, float y_angle, float z_angle){
		x_angle = Math::ToRad(x_angle);
		y_angle = Math::ToRad(y_angle);
		z_angle = Math::ToRad(z_angle);
		float sx = Math::Sin(x_angle);
		float sy = Math::Sin(y_angle);
		float sz = Math::Sin(z_angle); 
		float cx = Math::Cos(x_angle);
		float cy = Math::Cos(y_angle);
		float cz = Math::Cos(z_angle);
		return Matrix4x4(
			cz*cy,	cz*sy*sx-sz*cx,	cz*sy*cx+sz*sx,	0.f,
			sz*cy,	sz*sy*sx+cz*cx,	sz*sy*cx-cz*sx,	0.f,
			-sy,	cy*sx,			cy*cx,			0.f,
			0.f,	0.f,			0.f,			0.f);
	}

	Matrix4x4 Matrix4x4::Rotate(const Vector3& angle){
		return Rotate(angle.x, angle.y, angle.z);
	}

	Matrix4x4 Matrix4x4::Scale(const Vector3& s){
		return Matrix4x4(
			s.x, 0.f, 0.f, 0.f,
			0.f, s.y, 0.f, 0.f,
			0.f, 0.f, s.z, 0.f,
			0.f, 0.f, 0.f, 1.f
			);
	}

	Matrix4x4 Matrix4x4::LookAt(const Vector3& pEye, const Vector3& pTarget, const Vector3& up){
		Vector3 zaxis = (pEye - pTarget).Normalize();	// -z is the viewing direction in this coord system
		Vector3 xaxis = Cross(up, zaxis).Normalize();
		Vector3 yaxis = Cross(zaxis, xaxis);
		return Matrix4x4(
			xaxis.x,			yaxis.x,			zaxis.x,			0.f,
			xaxis.y,			yaxis.y,			zaxis.y,			0.f,
			xaxis.z,			yaxis.z,			zaxis.z,			0.f,
			Dot(xaxis, pEye),	Dot(yaxis, pEye),	Dot(zaxis, pEye),	1.f);
	}

	Vector3 Matrix4x4::TPoint(const Matrix4x4& m, const Vector3& p){
		return Vector3(
			p.x * m.m[0][0] + p.y * m.m[0][1] + p.z * m.m[0][2] + m.m[0][3],
			p.x * m.m[1][0] + p.y * m.m[1][1] + p.z * m.m[1][2] + m.m[1][3],
			p.x * m.m[2][0] + p.y * m.m[2][1] + p.z * m.m[2][2] + m.m[2][3]);
	}

	Vector3 Matrix4x4::TVector(const Matrix4x4& m, const Vector3& v){
		return Vector3(
			v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
			v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
			v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]);
	}

	Vector3 Matrix4x4::TNormal(const Matrix4x4& m_inv, const Vector3& n){
		return Vector3(
			m_inv.m[0][0] * n.x + m_inv.m[1][0] * n.y + m_inv.m[2][0] * n.z,
			m_inv.m[0][1] * n.x + m_inv.m[1][1] * n.y + m_inv.m[2][1] * n.z,
			m_inv.m[0][2] * n.x + m_inv.m[1][2] * n.y + m_inv.m[2][2] * n.z);
	}

	inline std::ostream& operator << (std::ostream& os, const Matrix4x4& m){
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++)
				os << m.m[i][j] << "\t";
			os << std::endl;
		}
		return os;
	}

	inline bool Matrix4x4::IsAffine(){
		return m[3][0] == 0.f && m[3][1] == 0.f && m[3][2] == 0.f && m[3][3] == 1.f;
	}

	inline Matrix4x4 Matrix4x4::InverseAffine(){
		float cf[4][4];
		float det, det_inv;
		// find 3x3 matrix inverse
		cf[0][0] = m[1][1] * m[2][2] - m[2][1] * m[1][2];
		cf[0][1] = m[0][2] * m[2][0] - m[2][2] * m[0][1];
		cf[0][2] = m[0][1] * m[1][2] - m[1][1] * m[0][2];
		det = m[0][0] * cf[0][0] + m[0][1] * cf[0][1] + m[0][2] * cf[0][2];
		if (Math::Abs(det) < Math::EPSILON){
			std::cerr << "not invertible:\n" << *this << std::endl;
			return Matrix4x4();		// not invertible
		} else {
			cf[1][0] = m[2][0] * m[1][2] - m[1][0] * m[2][2];
			cf[1][1] = m[0][0] * m[2][2] - m[2][0] * m[0][2];
			cf[1][2] = m[1][0] * m[0][2] - m[0][0] * m[1][2];
			cf[2][0] = m[1][0] * m[2][1] - m[2][0] * m[1][1];
			cf[2][1] = m[2][0] * m[0][1] - m[0][0] * m[2][1];
			cf[2][2] = m[0][0] * m[1][1] - m[1][0] * m[0][1];
			det_inv = 1.f / det;
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 3; j++)
					cf[i][j] *= det_inv;
				cf[i][3] = -m[i][3];	// invert the translation
			}
			cf[3][0] = cf[3][1] = cf[3][2] = 0.f;
			cf[3][3] = 1.f;
			return Matrix4x4(cf);
		}
	}

	inline Matrix4x4 Matrix4x4::InverseGeneral(){
		float res[4][4];
		float *src = *m, *dst = *res;
		// Intel AP-928
		__m128 minor0, minor1, minor2, minor3;
		__m128 row0, row1, row2, row3;
		__m128 det, tmp1;
		tmp1	= _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(src)), (__m64*)(src + 4));
		row1	= _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(src + 8)), (__m64*)(src + 12));
		row0	= _mm_shuffle_ps(tmp1, row1, 0x88);
		row1	= _mm_shuffle_ps(row1, tmp1, 0xDD);
		tmp1	= _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src + 2)), (__m64*)(src + 6));
		row3	= _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(src + 10)), (__m64*)(src + 14));
		row2	= _mm_shuffle_ps(tmp1, row3, 0x88);
		row3	= _mm_shuffle_ps(row3, tmp1, 0xDD);					//
		tmp1	= _mm_mul_ps(row2, row3);
		tmp1	= _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor0	= _mm_mul_ps(row1, tmp1);
		minor1	= _mm_mul_ps(row0, tmp1);
		tmp1	= _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0	= _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
		minor1	= _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
		minor1	= _mm_shuffle_ps(minor1, minor1, 0x4E);				//
		tmp1	= _mm_mul_ps(row1, row2);
		tmp1	= _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor0	= _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
		minor3	= _mm_mul_ps(row0, tmp1);
		tmp1	= _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0	= _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
		minor3	= _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
		minor3	= _mm_shuffle_ps(minor3, minor3, 0x4E);				//
		tmp1	= _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
		tmp1	= _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		row2	= _mm_shuffle_ps(row2, row2, 0x4E);
		minor0	= _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
		minor2	= _mm_mul_ps(row0, tmp1);
		tmp1	= _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0	= _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
		minor2	= _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
		minor2	= _mm_shuffle_ps(minor2, minor2, 0x4E);				//
		tmp1	= _mm_mul_ps(row0, row1);
		tmp1	= _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor2	= _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
		minor3	= _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
		tmp1	= _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor2	= _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
		minor3	= _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));		//
		tmp1	= _mm_mul_ps(row0, row3);
		tmp1	= _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor1	= _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
		minor2	= _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
		tmp1	= _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor1	= _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
		minor2	= _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));		//
		tmp1	= _mm_mul_ps(row0, row2);
		tmp1	= _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor1	= _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
		minor3	= _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
		tmp1	= _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor1	= _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
		minor3	= _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);		//
		det		= _mm_mul_ps(row0, minor0);
		det		= _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
		det		= _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
		tmp1	= _mm_rcp_ss(det);
		det		= _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
		det		= _mm_shuffle_ps(det, det, 0x00);
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