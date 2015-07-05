#include "stdafx.h"
namespace TX
{
	namespace Tests
	{
		using namespace SIMD;
		TEST_CLASS(SIMDTests){
		public:
			TEST_METHOD(ConstructorTest){
				Float4 f4 = Float4(1, 2, 1, 2);
				Assertions::AreClose(1, f4[0]);
				Assertions::AreClose(2, f4[1]);
				Assertions::AreClose(1, f4[2]);
				Assertions::AreClose(2, f4[3]);
				Assertions::AreClose(Float4(_mm_setzero_ps()), Float4());
				Assertions::AreClose(Float4(1), Float4(1, 1, 1, 1));
				Assertions::AreClose(Float4(1, 2), f4);
				Assertions::AreClose(Float4(f4), f4);
			}
			TEST_METHOD(ComparisonTest){
				Assert::IsTrue(Float4(1, 2, 3, 5) != Float4(1, 2, 3, 4));
				Assert::IsTrue(Float4(_mm_set_ps(4, 3, 2, 1)) == Float4(1, 2, 3, 4));
				Assert::IsTrue(Float4(32) == 32);
				Assert::IsTrue(Float4(100) != 0);
				repeat(n, 100){
					Float4 a = RandomFloat4(), b = RandomFloat4();
					Float4 min = Min(a, b), max = Max(a, b);
					repeat(i, 4){
						Assertions::AreClose(Math::Min(a[i], b[i]), min[i]);
						Assertions::AreClose(Math::Max(a[i], b[i]), max[i]);
					}
				}
			}
			TEST_METHOD(ArithTest){
				repeat(i, 100){
					Float4 a = RandomFloat4(), b = RandomFloat4();
					Float4 c, d, e, f, g;
					// unary
					c = +a; d = -a;
					repeat(j, 4){
						Assertions::AreClose(c[j], a[j]);
						Assertions::AreClose(d[j], -a[j]);
					}
					// binary
					c = a + b;
					d = a - b;
					e = a * b;
					f = a / b;
					repeat(j, 4){
						Assertions::AreClose(c[j], a[j] + b[j]);
						Assertions::AreClose(d[j], a[j] - b[j]);
						Assertions::AreClose(e[j], a[j] * b[j]);
						Assert::AreEqual(f[j], a[j] / b[j], 1.f); // low precision
					}
					g = a; g += b;
					Assertions::AreClose(g, c);
					g = a; g -= b;
					Assertions::AreClose(g, d);
					g = a; g *= b;
					Assertions::AreClose(g, e);
					g = a; g /= b;
					Assertions::AreClose(g, f);
				}
			}
			TEST_METHOD(MathTest){
				repeat(n, 100){
					Float4 a = RandomFloat4(), b = RandomFloat4();
					Float4 abs = Abs(a);
					Float4 exp = Exp(a);
					Float4 lg = Log(a);
					Float4 lg2 = Log2(a);
					Float4 lg10 = Log10(a);
					Float4 pw = Pow(a, b);
					repeat(i, 4){
						Assertions::AreClose(Math::Abs(a[i]), abs[i]);
						Assertions::AreClose(Math::Exp(a[i]), exp[i]);
						Assertions::AreClose(Math::Log(a[i]), lg[i]);
						Assertions::AreClose(Math::Log2(a[i]), lg2[i]);
						Assertions::AreClose(Math::Log10(a[i]), lg10[i]);
						Assertions::AreClose(Math::Pow(a[i], b[i]), pw[i]);
					}
				}
			}
			TEST_METHOD(TrigTest){
				repeat(n, 100){
					Float4 a = RandomFloat();
					Float4 torad = ToRad(a);
					Float4 todeg = ToDeg(a);
					Float4 sin = Sin(a);
					Float4 cos = Cos(a);
					Float4 tan = Tan(a);
					repeat(i, 4){
						Assertions::AreClose(Math::ToRad(a[i]), torad[i]);
						Assertions::AreClose(Math::ToDeg(a[i]), todeg[i]);
						Assertions::AreClose(Math::Sin(a[i]), sin[i]);
						Assertions::AreClose(Math::Cos(a[i]), cos[i]);
						Assertions::AreClose(Math::Tan(a[i]), tan[i]);
					}
				}
			}
			TEST_METHOD(RoundTest){
				repeat(n, 100){
					Float4 a = RandomFloat4();
					Float4 floor = Floor(a);
					Float4 ceil = Ceil(a);
					Float4 round = Round(a);
					repeat(i, 4){
						Assertions::AreClose(Math::Floor(a[i]), floor[i]);
						Assertions::AreClose(Math::Ceil(a[i]), ceil[i]);
						Assertions::AreClose(Math::Round(a[i]), round[i]);
					}
				}
			}
		};
	}
}