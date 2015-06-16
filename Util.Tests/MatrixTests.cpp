#include "stdafx.h"

namespace TX
{
	namespace Tests
	{
		TEST_CLASS(MatrixTests){
		public:
			TEST_METHOD(InverseTest){
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
					Vector3 sample = RandomVector();
					Vector3 translation = RandomVector();
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
				Assertions::AreClose(Vector3(0, 0, -1), Matrix4x4::TVector(m, input));
				Assertions::AreClose(Vector3(0, 1, 1), Matrix4x4::TNormal(m.Inverse(), input));

				// rotation around axis
				repeat(i, 100){
					float rad = RandomFloat(Math::TWO_PI);
					float deg = Math::ToDeg(rad);
					m = Matrix4x4::Rotate(deg, Vector3::Z);
					Assertions::AreClose(Matrix4x4::Rotate(0, 0, deg), m);
					Assertions::AreClose(Vector3(Math::Cos(rad), Math::Sin(rad), 0), Matrix4x4::TVector(m, input));
				}
			}

			TEST_METHOD(ScaleTest){
				repeat(i, 100){
					Vector3 sample = RandomVector();
					Vector3 scale = RandomVector();
					Vector3 expected(sample.x * scale.x, sample.y * scale.y, sample.z * scale.z);
					Matrix4x4 m = Matrix4x4::Scale(scale);
					Assertions::AreClose(expected, Matrix4x4::TPoint(m, sample));
					Assertions::AreClose(expected, Matrix4x4::TVector(m, sample));
				}
			}
		};
	}
}