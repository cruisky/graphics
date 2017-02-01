#include "txbase_tests/stdafx.h"

using namespace TX::Math;

namespace TX{
	namespace Tests
	{
		TEST_CLASS(MathUtilTests)
		{
		public:
			TEST_METHOD(RsqrtTest){
				repeat(i, 10){
					float f = RandomFloat(1e-2f);
					Assertions::AreClose(1.f / sqrtf(f), Rsqrt(f));
				}
			}
			TEST_METHOD(FloorTest){
				repeat(i, 10){
					float f = RandomFloat();
					Assertions::AreClose(floorf(f), Floor(f));
				}
			}
			TEST_METHOD(CeilTest){
				repeat(i, 10){
					float f = RandomFloat();
					Assertions::AreClose(ceilf(f), Ceil(f));
				}
			}
			TEST_METHOD(RoundTest){
				repeat(i, 10){
					float f = RandomFloat();
					Assertions::AreClose(roundf(f), Round(f));
				}
			}
		};
	}
}
