#include "txbase_tests/stdafx.h"

namespace TX
{
	namespace Tests
	{
		using namespace Math;
		TEST_CLASS(QuaternionTests) {
		public:
			TEST_METHOD(NormTest) {
				repeat(i, 10) {
					Quaternion q(RandomVec4());
					float norm = Sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
					Assertions::AreClose(norm, q.Norm());
				}
			}
			TEST_METHOD(ConjugateTest) {
				repeat(i, 10) {
					Vec4 v4 = RandomVec4();
					Quaternion q(v4);
					Quaternion qConjugate = q.Conjugate();
					Vec4 expected = v4 * Vec4(-1, -1, -1, 1);
					Assertions::AreClose(expected, qConjugate.q);
				}
			}
			TEST_METHOD(InverseTest) {
				repeat(i, 100) {
					Quaternion q(Normalize(RandomVec4()));
					Assertions::AreClose(Quaternion::IDENTITY, q * q.Inverse());
				}
			}
			TEST_METHOD(MultTest) {
				Quaternion inputLeft[] = {
					Quaternion(0.5, -3, 4, 1),
					Quaternion(0.18260, 0.54770, 0.73030, 0.36510),
					Quaternion(0.9889, 0.0, 0.0, 0.14834)
				};
				Quaternion inputRight[] = {
					Quaternion(2, 1, -9, 6),
					Quaternion(0.9889, 0.0, 0.0, 0.14834),
					Quaternion(0.18260, 0.54770, 0.73030, 0.36510)
				};
				Quaternion output[] = {
					Quaternion(28, -4.5, 21.5, 44),
					Quaternion(0.388127, 0.803418, -0.433317, -0.126429),
					Quaternion(0.388127, -0.64097, 0.649924, -0.126429)
				};
				repeat(i, sizeof(output) / sizeof(Quaternion)) {
					Assertions::AreClose(output[i], inputLeft[i] * inputRight[i], Str("i = ", i));
				}
			}
			TEST_METHOD(VectorRotationTest) {
				Vec3 inputVec[] = {
					Vec3(1, 1, 1),
					Vec3(1, 2, 3),
					Vec3(1, 0, 0),
					Vec3(1, 0, 0),
					Vec3(0, 1, 0),
					Vec3(0, 0, 1),
					Vec3(1, 0, 0),
					Vec3(0, 1, 0),
					Vec3(0, 0, 1),
					Vec3(1, 0, 0),
					Vec3(0, 1, 0),
					Vec3(0, 0, 1)
				};
				Quaternion inputQuat[] = {
					Quaternion(0, 1, 0, 1),
					Quaternion::IDENTITY,
					Quaternion(0.18260, 0.54770, 0.73030, 0.36510),
					Quaternion(1, 0, 0, 1),
					Quaternion(1, 0, 0, 1),
					Quaternion(1, 0, 0, 1),
					Quaternion(0, 1, 0, 1),
					Quaternion(0, 1, 0, 1),
					Quaternion(0, 1, 0, 1),
					Quaternion(0, 0, 1, 1),
					Quaternion(0, 0, 1, 1),
					Quaternion(0, 0, 1, 1)
				};
				Vec3 output[] = {
					Vec3(1, 1, -1),
					Vec3(1, 2, 3),
					Vec3(-0.6667, 0.7333, -0.1332),
					Vec3(1, 0, 0),
					Vec3(0, 0, 1),
					Vec3(0, -1, 0),
					Vec3(0, 0, -1),
					Vec3(0, 1, 0),
					Vec3(1, 0, 0),
					Vec3(0, 1, 0),
					Vec3(-1, 0, 0),
					Vec3(0, 0, 1)
				};
				repeat(i, sizeof(output) / sizeof(Vec3)) {
					inputQuat[i] = Normalize(inputQuat[i]);
					Assertions::AreClose(output[i], inputQuat[i].Rotate(inputVec[i]), Str("i=", i));
				}
			}
			TEST_METHOD(RotationMatrixTest) {
				repeat(i, 100) {
					Vec3 axis = Math::Normalize(RandomVec3());
					float angle = RandomFloat(0, PI, true);
					Quaternion q = Quaternion::AngleAxis(angle, axis);
					Matrix4x4 expected = Matrix4x4::Rotate(angle, axis);
					Assertions::AreClose(expected, q.RotationMatrix4x4(), Str("i=", i));
				}
				repeat(i, 100) {
					Quaternion q(Normalize(RandomVec4(1e-3f)));
					Assertions::AreClose(q, Quaternion::RotationMatrix(q.RotationMatrix4x4()), Str("quat=", q, "\ni=", i));
				}
			}
			TEST_METHOD(EulerAngleTest) {
				Vec3 eulers[] = {
					Vec3::ZERO,
					Vec3(0, 0, PI / 2),
					Vec3(PI, PI / 6, 0),
					Vec3(0, ToRad(70),ToRad(50)),

				};
				Quaternion quats[] = {
					Quaternion::IDENTITY,
					Quaternion(0, 0, 0.70711, 0.70711),
					Quaternion(0.965926, 0, -0.25882, 0),
					Quaternion(-0.2424, 0.519837, 0.346189, 0.7424)
				};

				repeat(i, sizeof(eulers) / sizeof(Vec3)) {
					Assertions::AreClose(quats[i], Quaternion::Euler(eulers[i]), Str("Euler -> Quaternion, i = ", i));
					Assertions::AreClose(eulers[i], quats[i].Euler(), Str("Quaternion -> Euler, i = ", i));
				}

				repeat(i, 100) {
					Vec3 euler = RandomVec3(0, Math::PI, true);
					Quaternion quat = Quaternion::Euler(euler);
					Quaternion converted = Quaternion::Euler(quat.Euler());
					Assertions::AreClose(quat, converted, Str("i=", i));
				}
			}
			TEST_METHOD(AngleAxisTest) {
				repeat(i, 100) {
					float angle = RandomFloat(0, PI, true);
					const Vec3& axis = Vec3::UNIT[RandomInt(0, 2)];
					Quaternion quat = Quaternion::AngleAxis(angle, axis);
					Assertions::AreClose(Matrix4x4::Rotate(angle, axis), quat.RotationMatrix4x4(), Str("quat=", quat));
				}
			}
			TEST_METHOD(FromToRotationTest) {
				repeat(i, 3) {
					const Vec3& from = Vec3::UNIT[i];
					repeat(j, 3) {
						const Vec3& to = Vec3::UNIT[j];
						auto quat = Quaternion::FromToRotation(from, to);
						Assertions::AreClose(
							to,
							quat.Rotate(from),
							Str("from=", from,
								"\nto=", to,
								"\nquat=", quat,
								"\neuler=", quat.Euler()));
					}
				}

				repeat(i, 100) {
					Vec3 from(Normalize(RandomVec3(1e-3f))), to(Normalize(RandomVec3(1e-3f)));
					Quaternion rotation = Quaternion::FromToRotation(from, to);

					Assertions::AreClose(to, rotation.Rotate(from), Str("i=", i, "\nquat=", rotation));
				}
			}
			TEST_METHOD(LookRotation) {
				repeat(i, 3) {
					const Vec3& targetDir = Vec3::UNIT[i];
					Quaternion q = Quaternion::LookRotation(targetDir, Vec3::UNIT[(i+1)%3]);
					Assertions::AreClose(targetDir, q.Rotate(Vec3::FORWARD), Str("quat=", q));
				}
				repeat(i, 100) {
					Vec3 targetDir = Normalize(RandomVec3(1e-3f));
					Quaternion lookAt = Quaternion::LookRotation(targetDir, Vec3::UP);
					Assertions::AreClose(targetDir, lookAt.Rotate(Vec3::FORWARD), Str("quat=", lookAt, "\ni=", i));
				}
			}
		};
	}
}
