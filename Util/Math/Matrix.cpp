#include "UtilStdAfx.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/Ray.h"

namespace TX{
	const Matrix3x3 Matrix3x3::IDENTITY = Matrix3x3(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f);


	const Matrix4x4 Matrix4x4::IDENTITY = Matrix4x4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f);

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
			a.x*a.x*c1 + c, a.x*a.y*c1 - a.z*s, a.x*a.z*c1 + a.y*s, 0.f,
			a.x*a.y*c1 + a.z*s, a.y*a.y*c1 + c, a.y*a.z*c1 - a.x*s, 0.f,
			a.x*a.z*c1 - a.y*s, a.y*a.z*c1 + a.x*s, a.z*a.z*c1 + c, 0.f,
			0.f, 0.f, 0.f, 1.f);
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
			Vector4(1.f / r, 0.f, 0.f, 0.f),
			Vector4(0.f, 1.f / t, 0.f, 0.f),
			Vector4(0.f, 0.f, 1.f / (af - an), (an + af) / (af - an)),
			Vector4::W);
	}

	Matrix4x4 Matrix4x4::Perspective(float ratio, float fov, float an, float af){
		assert(an > 0 && af > 0);
		float t = Math::Tan(Math::ToRad(fov) / 2.f) * an;		// top
		float r = ratio * t;									// right
		return Matrix4x4(
			Vector4(an / r, 0.f, 0.f, 0.f),
			Vector4(0.f, an / t, 0.f, 0.f),
			Vector4(0.f, 0.f, (an + af) / (an - af), 2 * af * an / (an - af)),
			Vector4(0.f, 0.f, -1.f, 0.f));
	}

	Matrix4x4 Matrix4x4::Viewport(float resx, float resy){
		resx *= 0.5f;
		resy *= 0.5f;
		// upside down screen space
		return Matrix4x4(
			Vector4(resx, 0.f, 0.f, resx - 0.5f),
			Vector4(0.f, resy, 0.f, resy - 0.5f),
			Vector4(0.f, 0.f, 1.f, 0.f),
			Vector4::W);

		//return Matrix4x4(
		//	resx, 0.f, 0.f, resx - 0.5f,
		//	0.f, -resy, 0.f, resy - 0.5f,
		//	0.f, 0.f, 1.f, 0.f,
		//	0.f, 0.f, 0.f, 1.f);
	}
}