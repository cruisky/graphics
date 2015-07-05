#include "Math/Vector.h"

namespace TX{
	const Vector4 Vector4::X(1.f, 0.f, 0.f, 0.f);
	const Vector4 Vector4::Y(0.f, 1.f, 0.f, 0.f);
	const Vector4 Vector4::Z(0.f, 0.f, 1.f, 0.f);
	const Vector4 Vector4::W(0.f, 0.f, 0.f, 1.f);

	const Vector4 Vector4::ZERO;
	const Vector4 Vector4::ONE(1.f);

	const Vector4 Vector4::PI(Math::PI);
	const Vector4 Vector4::QTR_PI(Math::QTR_PI);
	const Vector4 Vector4::HALF_PI(Math::HALF_PI);
	const Vector4 Vector4::TWO_PI(Math::TWO_PI);
	const Vector4 Vector4::FOUR_PI(Math::FOUR_PI);
	const Vector4 Vector4::PI_RCP(Math::PI_RCP);
	const Vector4 Vector4::HALF_PI_RCP(Math::HALF_PI_RCP);
	const Vector4 Vector4::TWO_PI_RCP(Math::TWO_PI_RCP);
	const Vector4 Vector4::FOUR_PI_RCP(Math::FOUR_PI_RCP);

	const Vector3 Vector3::ONE(1.f, 1.f, 1.f);

}