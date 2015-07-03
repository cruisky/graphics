#include "include/Matrix.h"
#include "include/Vector.h"
#include "include/Ray.h"

namespace TX{
	const Matrix4x4 Matrix4x4::IDENTITY = Matrix4x4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f);

	Matrix4x4::Matrix4x4() : Matrix4x4(IDENTITY){}

	Matrix4x4::Matrix4x4(const Matrix4x4& ot){
		memcpy(row, ot.row, 4 * sizeof(__m128));
	}

	Matrix4x4::Matrix4x4(float matrix[4][4]){
		row[0] = _mm_loadu_ps(matrix[0]);
		row[1] = _mm_loadu_ps(matrix[1]);
		row[2] = _mm_loadu_ps(matrix[2]);
		row[3] = _mm_loadu_ps(matrix[3]);
	}


	Matrix4x4::Matrix4x4(__m128 r0, __m128 r1, __m128 r2, __m128 r3){
		row[0] = r0; row[1] = r1; row[2] = r2; row[3] = r3;
	}

	Matrix4x4::Matrix4x4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33){
		row[0] = _mm_set_ps(m00, m01, m02, m03);
		row[1] = _mm_set_ps(m10, m11, m12, m13);
		row[2] = _mm_set_ps(m20, m21, m22, m23);
		row[3] = _mm_set_ps(m30, m31, m32, m33);
	}

	Matrix4x4 Matrix4x4::Transpose() const {
		__m128 m0 = _mm_unpacklo_ps(row[0], row[1]);
		__m128 m1 = _mm_unpacklo_ps(row[2], row[3]);
		__m128 m2 = _mm_unpackhi_ps(row[0], row[1]);
		__m128 m3 = _mm_unpackhi_ps(row[2], row[3]);
		return Matrix4x4(
			_mm_unpacklo_ps(m0, m1),
			_mm_unpackhi_ps(m0, m1),
			_mm_unpacklo_ps(m2, m3),
			_mm_unpackhi_ps(m2, m3));
	}

	Matrix4x4 Matrix4x4::Inverse() const {
		if (IsAffine()){
			return InverseAffine();
		} else {
			return InverseGeneral();
		}
	}

	Matrix4x4 Matrix4x4::InverseRotation() const {
		Matrix4x4 result(*this);
		// the inverse of rotation matrix is the same as the transpose
		result.InternalTranspose();
		return result.InternalApplyTranslation();
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
			0.f, 0.f, 0.f, 1.f);
	}

	Matrix4x4 Matrix4x4::Rotate(float x, float y, float z){
		return Rotate(Vector3(x, y, z));
	}

	Matrix4x4 Matrix4x4::LookAt(const Vector3& pEye, const Vector3& pTarget, const Vector3& up){
		assert(pEye != pTarget);
		Vector3 zaxis = (pEye - pTarget).Normalize();	// -z forward
		Vector3 xaxis = Cross(up, zaxis).Normalize();
		Vector3 yaxis = Cross(zaxis, xaxis);
		return Matrix4x4(
			xaxis.x, xaxis.y, xaxis.z, -Dot(xaxis, pEye),
			yaxis.x, yaxis.y, yaxis.z, -Dot(yaxis, pEye),
			zaxis.x, zaxis.y, zaxis.z, -Dot(zaxis, pEye),
			0.f, 0.f, 0.f, 1.f);
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

	Matrix4x4 Matrix4x4::Orthographic(float ratio, float size, float an, float af){
		assert(an > 0 && af > 0);
		float t = size;			// top
		float r = ratio * t;	// right
		return Matrix4x4(
			1.f / r, 0.f, 0.f, 0.f,
			0.f, 1.f / t, 0.f, 0.f,
			0.f, 0.f, 1.f / (af - an), (an + af) / (af - an),
			0.f, 0.f, 0.f, 1.f);
	}

	Matrix4x4 Matrix4x4::Perspective(float ratio, float fov, float an, float af){
		assert(an > 0 && af > 0);
		float t = Math::Tan(Math::ToRad(fov) / 2.f) * an;		// top
		float r = ratio * t;									// right
		return Matrix4x4(
			an / r, 0.f, 0.f, 0.f,
			0.f, an / t, 0.f, 0.f,
			0.f, 0.f, (an + af) / (an - af), 2 * af * an / (an - af),
			0.f, 0.f, -1.f, 0.f);
	}

	Matrix4x4 Matrix4x4::Viewport(float resx, float resy){
		resx *= 0.5f;
		resy *= 0.5f;
		// upside down screen space
		return Matrix4x4(
			resx, 0.f, 0.f, resx - 0.5f,
			0.f, resy, 0.f, resy - 0.5f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f);

		//return Matrix4x4(
		//	resx, 0.f, 0.f, resx - 0.5f,
		//	0.f, -resy, 0.f, resy - 0.5f,
		//	0.f, 0.f, 1.f, 0.f,
		//	0.f, 0.f, 0.f, 1.f);
	}

	Vector3 Matrix4x4::TPoint(const Matrix4x4& m, const Vector3& p){
		// apply translation
		return Vector3(
			p.x * m.m[0][0] + p.y * m.m[0][1] + p.z * m.m[0][2] + m.m[0][3],
			p.x * m.m[1][0] + p.y * m.m[1][1] + p.z * m.m[1][2] + m.m[1][3],
			p.x * m.m[2][0] + p.y * m.m[2][1] + p.z * m.m[2][2] + m.m[2][3]);
	}

	Vector3 Matrix4x4::TVector(const Matrix4x4& m, const Vector3& v){
		// ignore translation
		return Vector3(
			v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2],
			v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2],
			v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2]);
	}

	Vector3 Matrix4x4::TNormal(const Matrix4x4& m_inv, const Vector3& n){
		// multiply the transpose
		return Vector3(
			n.x * m_inv.m[0][0] + n.y * m_inv.m[1][0] + n.z * m_inv.m[2][0],
			n.x * m_inv.m[0][1] + n.y * m_inv.m[1][1] + n.z * m_inv.m[2][1],
			n.x * m_inv.m[0][2] + n.y * m_inv.m[1][2] + n.z * m_inv.m[2][2]);
	}
}
