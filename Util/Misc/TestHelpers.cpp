#include "Misc/TestHelpers.h"
#include "System/Tools.h"
#include "Graphics/Color.h"
#include "Graphics/Ray.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/SIMD.h"
#include "Math/RNG.h"

namespace TX
{
	namespace Tests {
		using namespace SIMD;
		RNG rng;

		float RandomFloat(bool allowzero, float absmax){
			float f;
			do{
				f = rng.Float();
			} while (!allowzero && f == 0.f);
			return f * absmax;
		}

		Float4 RandomFloat4(bool allowzero, float absmax){
			return Float4(
				RandomFloat(allowzero, absmax), 
				RandomFloat(allowzero, absmax), 
				RandomFloat(allowzero, absmax), 
				RandomFloat(allowzero, absmax));
		}

		Color RandomColor(float absmax){
			return Color(rng.Float(), rng.Float(), rng.Float()) * absmax;
		}

		Vector3 RandomVector(bool allowzero, float absmax){
			Vector3 v;
			do{
				v.x = (rng.Float() - 0.5f) * absmax;
			} while (!allowzero && Math::Abs(v.x) < 1e-6f);
			do{
				v.y = (rng.Float() - 0.5f) * absmax;
			} while (!allowzero && Math::Abs(v.y) < 1e-6f);
			do{
				v.z = (rng.Float() - 0.5f) * absmax;
			} while (!allowzero && Math::Abs(v.y) < 1e-6f);
			return v;
		}

		Ray RandomRay(float lengthmax){
			Vector3 origin = RandomVector();
			Vector3 dir = RandomVector(false, lengthmax);
			return Ray(origin, dir);
		}

		Matrix4x4 RandomMatrix(float absmax){
			float mat[4][4];
			repeat(i, 4){
				repeat(j, 4){
					mat[i][j] = RandomFloat(absmax);
				}
			}
			return Matrix4x4(mat);
		}

		namespace Assertions
		{
			void AreClose(const Float4& expected, const Float4& actual){
				repeat(i, 4){
					Assert::AreEqual(expected[i], actual[i], TOLERANCE_FLT, Msg::EQ(expected[i], actual[i]));
				}
			}

			void AreClose(const Color& expected, const Color& actual){
				Assert::AreEqual(expected.r, actual.r, TOLERANCE_FLT, Msg::EQ(expected, actual));
				Assert::AreEqual(expected.g, actual.g, TOLERANCE_FLT, Msg::EQ(expected, actual));
				Assert::AreEqual(expected.b, actual.b, TOLERANCE_FLT, Msg::EQ(expected, actual));
			}

			void AreClose(const Vector3& expected, const Vector3& actual){
				Assert::AreEqual(expected.x, actual.x, TOLERANCE_FLT, Msg::EQ(expected, actual));
				Assert::AreEqual(expected.y, actual.y, TOLERANCE_FLT, Msg::EQ(expected, actual));
				Assert::AreEqual(expected.z, actual.z, TOLERANCE_FLT, Msg::EQ(expected, actual));
			}

			void AreClose(const Matrix4x4& expected, const Matrix4x4& actual){
				repeat(i, 4){
					repeat(j, 4){
						Assert::AreEqual(expected[i][j], actual[i][j], TOLERANCE_FLT, Msg::EQ(expected, actual));
					}
				}
			}
		}
	}
}