#include "UtilStdAfx.h"
#include "Vector.h"

namespace TX {
	const Vector3 Vector3::X(1.f, 0.f, 0.f);
	const Vector3 Vector3::Y(0.f, 1.f, 0.f);
	const Vector3 Vector3::Z(0.f, 0.f, 1.f);
	const Vector3 Vector3::UNIT[3] = {
		Vector3(1.f, 0.f, 0.f),
		Vector3(0.f, 1.f, 0.f),
		Vector3(0.f, 0.f, 1.f)
	};
	const Vector3 Vector3::ZERO(0.f);
	const Vector3 Vector3::ONE(1.f);
	const Vector3 Vector3::PI(Math::PI);
	const Vector3 Vector3::PI_RCP(Math::PI_RCP);

	const Vector4 Vector4::X(1.f, 0.f, 0.f, 0.f);
	const Vector4 Vector4::Y(0.f, 1.f, 0.f, 0.f);
	const Vector4 Vector4::Z(0.f, 0.f, 1.f, 0.f);
	const Vector4 Vector4::W(0.f, 0.f, 0.f, 1.f);
	const Vector4 Vector4::UNIT[4] = {
		Vector4(1.f, 0.f, 0.f, 0.f),
		Vector4(0.f, 1.f, 0.f, 0.f),
		Vector4(0.f, 0.f, 1.f, 0.f),
		Vector4(0.f, 0.f, 0.f, 1.f),
	};
	const Vector4 Vector4::ZERO(0.f);
	const Vector4 Vector4::ONE(1.f);
	const Vector4 Vector4::PI(Math::PI);
	const Vector4 Vector4::PI_RCP(Math::PI_RCP);
}