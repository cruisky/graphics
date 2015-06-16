#include "stdafx.h"

namespace TX
{
	namespace UtilTests
	{
		TEST_CLASS(ColorTests){
		public:
			RNG rng;
			Color RandomColor(){
				return Color(rng.Float(), rng.Float(), rng.Float());
			}

			TEST_METHOD(ClampTest){
				repeat(i, 100){
					Color c = RandomColor() *= 10;
					c.Clamp();
					Assert::IsTrue(0 <= c.r && c.r <= 1);
					Assert::IsTrue(0 <= c.g && c.g <= 1);
					Assert::IsTrue(0 <= c.b && c.b <= 1);
				}
			}
		};
	}
}