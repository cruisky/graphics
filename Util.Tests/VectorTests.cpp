#include "stdafx.h"

namespace TX{
	namespace Tests
	{
		TEST_CLASS(Vector3Tests)
		{
		public:
			TEST_METHOD(ArithTest){
				repeat(i, 100){
					float f = RandomFloat();
					Vector3 a = RandomVector3(), b = RandomVector3();
					Vector3 sum = a + b,
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
					Assertions::AreClose(1.f, RandomVector3(false).Normalize().Length());
				}
			}
			TEST_METHOD(NormalizedTest){
				repeat(i, 100){
					Assertions::AreClose(1.f, Normalized(RandomVector3(false)).Length());
				}
			}
			TEST_METHOD(DotTest){
				repeat(i, 100){
					Vector3 u = RandomVector3();
					Vector3 v = RandomVector3();
					float expected = u.x * v.x + u.y * v.y + u.z * v.z;
					Assert::AreEqual(expected, Dot(u, v));
					Assert::AreEqual(Math::Abs(expected), AbsDot(u, v));
					Assert::IsTrue(AbsDot(u, v) >= 0);
				}
			}
			TEST_METHOD(CrossTest){
				repeat(i, 100){
					Vector3 u = RandomVector3(false);
					Vector3 v = RandomVector3(false);
					Vector3 w = Cross(u, v);
					Assert::AreEqual(0, Dot(w, v), 2e-1f);
					Assert::AreEqual(0, Dot(w, u), 2e-1f);
				}
			}
			TEST_METHOD(LengthTest){
				repeat(i, 100){
					Vector3 v = RandomVector3();
					float expected = Dot(v, v);
					Assertions::AreClose(expected, v.LengthSqr());
					Assertions::AreClose(Math::Sqrt(expected), v.Length());
				}
			}
		};
		TEST_CLASS(Vector4Tests){
		public:
			TEST_METHOD(ArrayAccessTest){
				Vector4 v = Vector4(0, 1, 2, 3);
				repeat(i, 4){
					Assertions::AreClose(i, v[i]);
				}
			}
		};
	}
}