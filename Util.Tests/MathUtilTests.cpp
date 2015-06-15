#include "stdafx.h"

using namespace TX::Math;

namespace TX{
	namespace UtilTests
	{
		TEST_CLASS(MathUtilTests)
		{
		public:
			TEST_METHOD(RsqrtTest){
				float input[5] = {
					1.f,
					3746289.f,
					1e10f,
					2.7182818f,
					3.1415926f
				};
				float output[5] = {
					1.f,
					5.1665348e-4f,
					1e-5f,
					0.6065306f,
					0.5641895f
				};
				for (int i = 0; i < 5; i++){
					Assertions::AreClose(output[i], Rsqrt(input[i]));
				}
			}
			TEST_METHOD(FloorTest){
				Assert::AreEqual(0, Floor(0.1f));
				Assert::AreEqual(1, Floor(1.f));
				Assert::AreEqual(2, Floor(2 + 1e-6f));
				Assert::AreEqual(3, Floor(PI));
				Assert::AreEqual(4, Floor(5.f - 1e-6f));
			}
			TEST_METHOD(CeilTest){
				Assert::AreEqual(0, Ceil(0.f));
				Assert::AreEqual(1, Ceil(0.1f));
				Assert::AreEqual(2, Ceil(2.f));
				Assert::AreEqual(3, Ceil(2.f + 1e-6f));
				Assert::AreEqual(4, Ceil(Math::PI));
				Assert::AreEqual(5, Ceil(5.f - 1e-6f));
			}
			TEST_METHOD(RoundTest){
				Assert::AreEqual(0, Round(1e-6f));
				Assert::AreEqual(1, Round(1.f));
				Assert::AreEqual(2, Round(2.f - 1e-6f));
				Assert::AreEqual(3, Round(Math::PI));
			}
		};
	}
}