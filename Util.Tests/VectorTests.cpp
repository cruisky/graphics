#include "stdafx.h"

namespace TX{
	namespace Tests
	{
		using namespace Math;
		TEST_CLASS(Vec3Tests)
		{
		public:
			TEST_METHOD(ArithTest){
				repeat(i, 100){
					float f = RandomFloat();
					Vec3 a = RandomVec3(), b = RandomVec3();
					Vec3 sum = a + b,
						diff = a - b, 
						prod = a * f, 
						quot = a / f;
					Assertions::AreClose(a.x + b.x, sum.x);
					Assertions::AreClose(a.y - b.y, diff.y);
					Assertions::AreClose(a.z * f, prod.z);
					Assertions::AreClose(a.z / f, quot.z);
				}
			}

			TEST_METHOD(NormalizeTest){
				repeat(i, 100){
					Assertions::AreClose(1.f, Length(Normalize(RandomVec3(false))));
				}
			}
			TEST_METHOD(NormalizedTest){
				repeat(i, 100){
					Assertions::AreClose(1.f, Length(Normalize(RandomVec3(false))));
				}
			}
			TEST_METHOD(DotTest){
				repeat(i, 100){
					Vec3 u = RandomVec3();
					Vec3 v = RandomVec3();
					float expected = u.x * v.x + u.y * v.y + u.z * v.z;
					Assert::AreEqual(expected, Dot(u, v));
					Assert::AreEqual(Math::Abs(expected), AbsDot(u, v));
					Assert::IsTrue(AbsDot(u, v) >= 0);
				}
			}
			TEST_METHOD(CrossTest){
				repeat(i, 100){
					Vec3 u = RandomVec3(false);
					Vec3 v = RandomVec3(false);
					Vec3 w = Cross(u, v);
					Assert::AreEqual(0, Dot(w, v), 2e-1f);
					Assert::AreEqual(0, Dot(w, u), 2e-1f);
				}
			}
			TEST_METHOD(LengthTest){
				repeat(i, 100){
					Vec3 v = RandomVec3();
					float expected = Dot(v, v);
					Assertions::AreClose(expected, LengthSqr(v));
					Assertions::AreClose(Math::Sqrt(expected), Length(v));
				}
			}
		};
		TEST_CLASS(Vec4Tests){
		public:
			TEST_METHOD(ArrayAccessTest){
				Vec4 v = Vec4(0, 1, 2, 3);
				repeat(i, 4){
					Assertions::AreClose(i, v[i]);
				}
			}
		};
	}
}