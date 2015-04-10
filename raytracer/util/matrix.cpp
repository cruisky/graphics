#include "util/matrix.h"
#include "util/vector.h"
#include "util/ray.h"
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

	Matrix4x4 Matrix4x4::Transpose() const {
		return Matrix4x4(
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]
			);
	}
		
	Matrix4x4 Matrix4x4::Inverse() const {
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

	Matrix4x4 Matrix4x4::Translate(float x, float y, float z){
		return Translate(Vector3(x, y, z));
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

	Matrix4x4 Matrix4x4::Rotate(const Vector3& angle){
		float x = Math::ToRad(angle.x);
		float y = Math::ToRad(angle.y);
		float z = Math::ToRad(angle.z);
		float sx = Math::Sin(x);
		float sy = Math::Sin(y);
		float sz = Math::Sin(z);
		float cx = Math::Cos(x);
		float cy = Math::Cos(y);
		float cz = Math::Cos(z);
		return Matrix4x4(
			cz*cy, cz*sy*sx - sz*cx, cz*sy*cx + sz*sx, 0.f,
			sz*cy, sz*sy*sx + cz*cx, sz*sy*cx - cz*sx, 0.f,
			-sy, cy*sx, cy*cx, 0.f,
			0.f, 0.f, 0.f, 0.f);
	}

	Matrix4x4 Matrix4x4::Rotate(float x, float y, float z){
		return Rotate(Vector3(x, y, z));
	}

	Matrix4x4 Matrix4x4::LookAt(const Vector3& pEye, const Vector3& pTarget, const Vector3& up){
		Vector3 zaxis = (pEye - pTarget).Normalize();	// -z is the viewing direction in this coord system
		Vector3 xaxis = Cross(up, zaxis).Normalize();
		Vector3 yaxis = Cross(zaxis, xaxis);
		return Matrix4x4(
			xaxis.x, yaxis.x, zaxis.x, 0.f,
			xaxis.y, yaxis.y, zaxis.y, 0.f,
			xaxis.z, yaxis.z, zaxis.z, 0.f,
			Dot(xaxis, pEye), Dot(yaxis, pEye), Dot(zaxis, pEye), 1.f);
	}

	Matrix4x4 Matrix4x4::Scale(const Vector3& s){
		return Matrix4x4(
			s.x, 0.f, 0.f, 0.f,
			0.f, s.y, 0.f, 0.f,
			0.f, 0.f, s.z, 0.f,
			0.f, 0.f, 0.f, 1.f
			);
	}

	Matrix4x4 Matrix4x4::Scale(float x, float y, float z){
		return Scale(Vector3(x, y, z));
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
}