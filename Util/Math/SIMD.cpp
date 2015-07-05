#include "Math/SIMD.h"
#include "Math/MathUtil.h"

namespace TX
{
	namespace SIMD
	{
		const __m128 SIGN_ALL = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
		const __m128 SIGN_0 = _mm_castsi128_ps(_mm_set_epi32(0x80000000, 0, 0, 0));
		const __m128 SIGN_1 = _mm_castsi128_ps(_mm_set_epi32(0, 0x80000000, 0, 0));
		const __m128 SIGN_2 = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0x80000000, 0));
		const __m128 SIGN_3 = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, 0x80000000));
		
		const Float4 Float4::ZERO;
		const Float4 Float4::ONE(1.f);
		const Float4 Float4::PI(Math::PI);
		const Float4 Float4::QTR_PI(Math::QTR_PI);
		const Float4 Float4::HALF_PI(Math::HALF_PI);
		const Float4 Float4::TWO_PI(Math::TWO_PI);
		const Float4 Float4::FOUR_PI(Math::FOUR_PI);
		const Float4 Float4::PI_RCP(Math::PI_RCP);
		const Float4 Float4::HALF_PI_RCP(Math::HALF_PI_RCP);
		const Float4 Float4::TWO_PI_RCP(Math::TWO_PI_RCP);
		const Float4 Float4::FOUR_PI_RCP(Math::FOUR_PI_RCP);
	}
}