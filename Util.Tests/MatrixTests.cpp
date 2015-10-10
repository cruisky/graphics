#include "stdafx.h"

namespace TX
{
	namespace Tests
	{
		TEST_CLASS(MatrixTests){
		public:
			TEST_METHOD(MultTest){
				Matrix4x4 a(
					1, 2, 3, 4,
					2, 1, 2, 3,
					3, 2, 1, 2,
					4, 3, 2, 1);
				Matrix4x4 b(
					1, 0, 1, 0,
					0, 1, 0, 1,
					-1, 0, -1, 0,
					0, -1, 0, -1);
				Matrix4x4 expected(
					-2, -2, -2, -2,
					0, -2, 0, -2,
					2, 0, 2, 0,
					2, 2, 2, 2);
				Assertions::AreClose(expected, a*b);
			}

			TEST_METHOD(InverseTest){
				Matrix4x4 example(
					1, 2, 3, 4,
					1e-4, 1, 2, 3,
					1e-4, 1e-4, 1, 2,
					1e-4, 1e-4, 1e-4, 1);
				Matrix4x4 inverse(
					1, -2, 1, 0,
					0, 1, -2, 1,
					0, 0, 1, -2,
					0, 0, 0, 1);
				Matrix4x4 res = example.Inverse();
				Assertions::AreClose(inverse, res);

				Matrix4x4 t = Matrix4x4(
					0, 0, 1, 0,
					0, 1, 0, 0,
					-1, 0, 0, 0,
					0, 0, 0, 1);
				Msg::Log(t);
				Msg::Log(t.Inverse());
				repeat(i, 100){
					Matrix4x4 m = RandomMatrix();
					Matrix4x4 m_i = m.Inverse();
					Assertions::AreClose(Matrix4x4::IDENTITY, m * m_i);
					Assertions::AreClose(Matrix4x4::IDENTITY, m_i * m);
				}
			}

			TEST_METHOD(TransposeTest){
				repeat(n, 100){
					Matrix4x4 m = RandomMatrix();
					Matrix4x4 m_t = m.Transpose();
					Matrix4x4 sum = m + m_t;
					repeat(i, 4){
						repeat(j, i){
							Assertions::AreClose(sum[j][i], sum[i][j]);
						}
					}
				}
			}

			TEST_METHOD(TranslateTest){
				repeat(i, 100){
					Vector3 sample = RandomVector3();
					Vector3 translation = RandomVector3();
					Matrix4x4 m = Matrix4x4::Translate(translation);
					Assertions::AreClose(sample + translation, Matrix4x4::TPoint(m, sample));
					Assertions::AreClose(sample, Matrix4x4::TVector(m, sample));	// vector should not change
					Assertions::AreClose(sample, Matrix4x4::TNormal(m, sample));	// normal should not change
				}
			}

			TEST_METHOD(RotateTest){
				Matrix4x4 m, n;
				Vector3 input = Vector3::X;

				// euler rotation
				m = Matrix4x4::Rotate(0, 90, 0);
				Assertions::AreClose(m, Matrix4x4::Rotate(0, 90, 0));
				Assertions::AreClose(Vector3(0, 0, -1), Matrix4x4::TVector(m, input));
				Assertions::AreClose(Vector3(0, 0, -1), Matrix4x4::TNormal(m.Inverse(), input));

				// rotation around axis
				repeat(i, 100){
					float rad = RandomFloat(0, 2 * Math::PI);
					float deg = Math::ToDeg(rad);
					m = Matrix4x4::Rotate(deg, Vector3::Z);
					n = Matrix4x4::Rotate(deg, Vector3::X);
					Assertions::AreClose(Matrix4x4::Rotate(0, 0, deg), m);
					Assertions::AreClose(Matrix4x4::Rotate(deg, 0, 0), n);
					Assertions::AreClose(Vector3(Math::Cos(rad), Math::Sin(rad), 0), Matrix4x4::TVector(m, input));
				}
			}

			TEST_METHOD(ScaleTest){
				repeat(i, 100){
					Vector3 sample = RandomVector3();
					Vector3 scale = RandomVector3();
					Vector3 expected(sample.x * scale.x, sample.y * scale.y, sample.z * scale.z);
					Matrix4x4 m = Matrix4x4::Scale(scale);
					Assertions::AreClose(expected, Matrix4x4::TPoint(m, sample));
					Assertions::AreClose(expected, Matrix4x4::TVector(m, sample));
				}
			}
		};
	}
}