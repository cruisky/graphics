#include "stdafx.h"
namespace TX
{
	namespace Tests
	{
		TEST_CLASS(SIMDTests){
		public:
			TEST_METHOD(AlignmentTest){
				Assert::AreEqual(16, (int)std::alignment_of<Vector4>::value);
				Assert::AreEqual(16, (int)std::alignment_of<Vector3>::value);
				Assert::AreEqual(16, (int)std::alignment_of<Vector4>::value);
				Assert::AreEqual(16, (int)std::alignment_of<Ray>::value);
			}
			TEST_METHOD(ConstructorTest){
				Vector4 f4(1, 2, 1, 2);
				Assertions::AreClose(1, f4[0]);
				Assertions::AreClose(2, f4[1]);
				Assertions::AreClose(1, f4[2]);
				Assertions::AreClose(2, f4[3]);
				Assertions::AreClose(Vector4(_mm_setzero_ps()), Vector4());
				Assertions::AreClose(Vector4(1), Vector4(1, 1, 1, 1));
				Assertions::AreClose(Vector4(1, 2), f4);
				Assertions::AreClose(Vector4(f4), f4);
				Assert::IsTrue(Shuffle<3, 2, 1, 0>(f4) == _mm_set_ps(1, 2, 1, 2));
			}
			TEST_METHOD(ComparisonTest){
				Assert::IsTrue(Vector4(1, 2, 3, 5) != Vector4(1, 2, 3, 4));
				Assert::IsTrue(Vector4(_mm_set_ps(4, 3, 2, 1)) == Vector4(1, 2, 3, 4));
				Assert::IsTrue(Vector4(32) == 32);
				Assert::IsTrue(Vector4(100) != 0);
				repeat(n, 100){
					Vector4 a = RandomVector4(), b = RandomVector4();
					Vector4 min = Min(a, b), max = Max(a, b);
					repeat(i, 4){
						Assertions::AreClose(Math::Min(a[i], b[i]), min[i]);
						Assertions::AreClose(Math::Max(a[i], b[i]), max[i]);
					}
				}
			}
			TEST_METHOD(ArithTest){
				repeat(i, 100){
					Vector4 a = RandomVector4(), b = RandomVector4();
					Vector4 c, d, e, f, g;
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
					Vector4 pos = RandomVector4(0.1f, 5.f, false);
					Vector4 rand1 = RandomVector4(0.1f, 5.f, true), rand2 = RandomVector4(0.1f, 2.f, true);
					Vector4 abs = Abs(rand1);
					Vector4 exp = Exp(rand1);
					Vector4 lg = Log(pos);
					Vector4 lg2 = Log2(pos);
					Vector4 lg10 = Log10(pos);
					Vector4 pw = Pow(pos, rand2);
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
					Vector4 a = RandomFloat();
					Vector4 torad = ToRad(a);
					Vector4 todeg = ToDeg(a);
					Vector4 sin = Sin(a);
					Vector4 cos = Cos(a);
					Vector4 tan = Tan(a);
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
					Vector4 a = RandomVector4();
					Vector4 floor = Floor(a);
					Vector4 ceil = Ceil(a);
					Vector4 round = Round(a);
					repeat(i, 4){
						Msg::Log(a[i]);
						Assertions::AreClose((float)Math::Floor(a[i]), floor[i]);
						Assertions::AreClose((float)Math::Ceil(a[i]), ceil[i]);
						Assertions::AreClose((float)Math::Round(a[i]), round[i]);
					}
				}
			}
		};
	}
}