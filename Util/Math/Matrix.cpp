#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Graphics/Ray.h"

namespace TX{
	const Matrix4x4 Matrix4x4::IDENTITY = Matrix4x4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f);

	Matrix4x4 Matrix4x4::Translate(const Vector3& v){
		//return Matrix4x4(
		//	1.f, 0.f, 0.f, v.x,
		//	0.f, 1.f, 0.f, v.y,
		//	0.f, 0.f, 1.f, v.z,
		//	0.f, 0.f, 0.f, 1.f
		//	);
		return Matrix4x4(
			Insert<3,0,0>(Vector4::X, v),	// v0 -> x3
			Insert<3,1,0>(Vector4::Y, v),	// v1 -> y3
			Insert<3,2,0>(Vector4::Z, v),	// v2 -> z3
			Vector4::W);
	}

	Matrix4x4 Matrix4x4::Translate(float x, float y, float z){
		return Translate(Vector3(x, y, z));
	}

	Matrix4x4 Matrix4x4::Rotate(float angle, const Vector3& axis){
		Vector3 a = Normalized(axis);
		angle = Math::ToRad(angle);
		//float c = Math::Cos(angle), s = Math::Sin(angle), c1 = 1 - c;
		Vector4 tmp = Math::Sin(angle) * a;							// sin * axis (sa)
		tmp.w = Math::Cos(angle);									// tmp: sinaxis.x, sinaxis.y, sinaxis.z, cos
		Vector4 r0 = Shuffle<3, 2, 1, 3>(Negate<0,0,1,0>(tmp));		// cos, -sa.z, sa.y, cos
		Vector4 r1 = Shuffle<2, 3, 0, 3>(Negate<1,0,0,0>(tmp));		// sa.z, cos, -sa.x, cos
		Vector4 r2 = Shuffle<1, 0, 3, 3>(Negate<0,1,0,0>(tmp));		// -sa.y, sa.x, cos, cos
		r0.w = r1.w = r2.w = 0.f;
		tmp = Vector3::ONE - tmp.w;									// 1-cos, 1-cos, 1-cos, 0
		r0 += Shuffle<0, 0, 0, 3>(a) * a * tmp;						// a.x * a * (1-c)
		r1 += Shuffle<1, 1, 1, 3>(a) * a * tmp;						// a.y * a * (1-c)
		r2 += Shuffle<2, 2, 2, 3>(a) * a * tmp;						// a.z * a * (1-c)
		return Matrix4x4(r0, r1, r2, Vector4::W);
	}

	Matrix4x4 Matrix4x4::Rotate(const Vector3& angle){
		Vector3 tmp = ToRad(angle);
		Vector3 sin = Sin(tmp);		// 0, 1, 0, 0
		Vector3 cos = Cos(tmp);		// 1, -1, 1, 1
		Vector3 ssin = sin * Shuffle<1, 2, 0, 3>(sin);	// xy, yz, zx 
		Vector3 ccos = cos * Shuffle<1, 2, 0, 3>(cos);	// xy, yz, zx
		tmp = Vector3(cos.y, ssin[0], sin.y * cos.x);
		return Matrix4x4(
			Shuffle<2, 2, 2, 3>(cos) * tmp + Vector3(0.f, -sin.z * cos.x, ssin[2]),
			Shuffle<2, 2, 2, 3>(sin) * tmp + Vector3(0.f, ccos[2], -cos.z*sin.x),
			Vector3(-sin.y, cos.y * sin.x, ccos[0]),
			Vector4::W);
	}

	Matrix4x4 Matrix4x4::Rotate(float x, float y, float z){
		return Rotate(Vector3(x, y, z));
	}
	Matrix4x4 Matrix4x4::LookAt(const Vector3& pEye, const Vector3& pTarget, const Vector3& up){
		assert(pEye != pTarget);
		Vector3 zaxis = (pEye - pTarget).Normalize();	// -z forward
		Vector3 xaxis = Cross(up, zaxis).Normalize();
		Vector3 yaxis = Cross(zaxis, xaxis);
		xaxis.w = -Dot(xaxis, pEye);
		yaxis.w = -Dot(yaxis, pEye);
		zaxis.w = -Dot(zaxis, pEye);
		return Matrix4x4(xaxis, yaxis, zaxis, Vector4::W);
	}

	Matrix4x4 Matrix4x4::Scale(const Vector3& s){
		return Matrix4x4(
			Blend<1, 0, 0, 0>(Vector4(), s),
			Blend<0, 1, 0, 0>(Vector4(), s),
			Blend<0, 0, 1, 0>(Vector4(), s),
			Vector4::W);
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