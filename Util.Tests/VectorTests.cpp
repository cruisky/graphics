#include "stdafx.h"

namespace TX{
	namespace Tests
	{
		TEST_CLASS(VectorTests)
		{
		public:
			TEST_METHOD(NormalizeTest){
				repeat(i, 100){
					Assertions::AreClose(1.f, RandomVector(false).Normalize().Length());
				}
			}
			TEST_METHOD(NormalizedTest){
				repeat(i, 100){
					Assertions::AreClose(1.f, Normalized(RandomVector(false)).Length());
				}
			}
			TEST_METHOD(DotTest){
				repeat(i, 100){
					Vector3 u = RandomVector();
					Vector3 v = RandomVector();
					float expected = u.x * v.x + u.y * v.y + u.z * v.z;
					Assert::AreEqual(expected, Dot(u, v));
					Assert::AreEqual(Math::Abs(expected), AbsDot(u, v));
					Assert::IsTrue(AbsDot(u, v) >= 0);
				}
			}
			TEST_METHOD(CrossTest){
				repeat(i, 100){
					Vector3 u = RandomVector(false);
					Vector3 v = RandomVector(false);
					Vector3 w = Cross(u, v);
					Assert::AreEqual(0, Dot(w, v), 1e-1f);
					Assert::AreEqual(0, Dot(w, u), 1e-1f);
				}
			}
			TEST_METHOD(LengthTest){
				repeat(i, 100){
					Vector3 v = RandomVector();
					float expected = Dot(v, v);
					Assertions::AreClose(expected, v.LengthSqr());
					Assertions::AreClose(Math::Sqrt(expected), v.Length());
				}
			}
		};
	}
}