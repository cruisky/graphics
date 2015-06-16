#include "stdafx.h"

using namespace TX::Math;

namespace TX{
	namespace UtilTests
	{
		TEST_CLASS(MathUtilTests)
		{
		public:
			RNG rng;
			float RandomFloat(bool allowzero = true){
				float f;
				do{
					f = rng.Float();
				} while (!allowzero && f == 0.f);
				return f;
			}

			TEST_METHOD(RsqrtTest){
				repeat(i, 10){
					float f = RandomFloat(false);
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