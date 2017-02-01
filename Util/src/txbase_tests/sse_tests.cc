#include "txbase_tests/stdafx.h"
namespace TX
{
	namespace Tests
	{
		using namespace TX::SSE;

		TEST_CLASS(SIMDTests){
		public:
			TEST_METHOD(ConstructorTest){
				V4Float f4(1, 2, 1, 2);
				Assertions::AreClose(1, f4[0]);
				Assertions::AreClose(2, f4[1]);
				Assertions::AreClose(1, f4[2]);
				Assertions::AreClose(2, f4[3]);
				Assertions::AreClose(V4Float(_mm_setzero_ps()), V4Float());
				Assertions::AreClose(V4Float(1), V4Float(1, 1, 1, 1));
				Assertions::AreClose(V4Float(1, 2), f4);
				Assertions::AreClose(V4Float(f4), f4);
				Assert::IsTrue(All(Shuffle<3, 2, 1, 0>(f4) == _mm_set_ps(1, 2, 1, 2)));
			}
			TEST_METHOD(ComparisonTest){
				Assert::IsTrue(Any(V4Float(1, 2, 3, 5) != V4Float(1, 2, 3, 4)));
				Assert::IsTrue(All(V4Float(_mm_set_ps(4, 3, 2, 1)) == V4Float(1, 2, 3, 4)));
				Assert::IsTrue(All(V4Float(32) == 32));
				Assert::IsTrue(All(V4Float(100) != 0));
				repeat(n, 100){
					V4Float a = RandomV4Float(), b = RandomV4Float();
					V4Float min = Min(a, b), max = Max(a, b);
					repeat(i, 4){
						Assertions::AreClose(Math::Min(a[i], b[i]), min[i]);
						Assertions::AreClose(Math::Max(a[i], b[i]), max[i]);
					}
				}
			}
			TEST_METHOD(ArithTest){
				repeat(i, 100){
					V4Float a = RandomV4Float(), b = RandomV4Float();
					V4Float c, d, e, f, g;
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
					V4Float pos = RandomV4Float(0.1f, 5.f, false);
					V4Float rand1 = RandomV4Float(0.1f, 5.f, true), rand2 = RandomV4Float(0.1f, 2.f, true);
					V4Float abs = Abs(rand1);
					V4Float exp = Exp(rand1);
					V4Float lg = Log(pos);
					V4Float lg2 = Log2(pos);
					V4Float lg10 = Log10(pos);
					V4Float pw = Pow(pos, rand2);
					repeat(i, 4){
						Assertions::AreClose(Math::Abs(rand1[i]), abs[i]);
						Assertions::AreClose(Math::Exp(rand1[i]), exp[i]);
						Assertions::AreClose(Math::Log(pos[i]), lg[i]);
						Assertions::AreClose(Math::Log2(pos[i]), lg2[i]);
						Assertions::AreClose(Math::Log10(pos[i]), lg10[i]);
						Assert::AreEqual(Math::Pow(pos[i], rand2[i]), pw[i], 1.f);
						Assertions::AreClose(Math::Pow(pos[i], rand2[i]), pw[i]);
					}
				}
			}
			TEST_METHOD(TrigTest){
				repeat(n, 100){
					V4Float a = RandomFloat();
					V4Float torad = ToRad(a);
					V4Float todeg = ToDeg(a);
					V4Float sin = Sin(a);
					V4Float cos = Cos(a);
					V4Float tan = Tan(a);
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
					V4Float a = RandomV4Float();
					V4Float floor = Floor(a);
					V4Float ceil = Ceil(a);
					V4Float round = Round(a);
					repeat(i, 4){
						Msg::Log(a[i]);
						Assertions::AreClose((float)Math::Floor(a[i]), floor[i]);
						Assertions::AreClose((float)Math::Ceil(a[i]), ceil[i]);
						Assertions::AreClose((float)Math::Round(a[i]), round[i]);
					}
				}
			}
			TEST_METHOD(ReductionTest) {
				V4Float f = RandomV4Float();
				auto minIdx = SelectMin(f);
				auto maxIdx = SelectMax(f);
				Msg::Log(f);
				Msg::Log(minIdx);
				Msg::Log(maxIdx);
				Assertions::AreEqual(Math::Min(f[0], f[1], f[2], f[3]), f[minIdx], Str(f));
				Assertions::AreEqual(Math::Max(f[0], f[1], f[2], f[3]), f[maxIdx], Str(f));
			}
		};
	}
}
