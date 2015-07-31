#pragma once

#include "Graphics/Color.h"
#include "Graphics/Ray.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Random.h"

namespace TX
{
	namespace Tests
	{
		extern RNG rng;

		inline float RandomFloat(float absmin = 1e-6f, float absmax = 1.f, bool bothsign = false){
			float f;
			do{
				f = rng.Float() - 0.5f;
			} while (Math::Abs(f) < (absmin / absmax));
			f = bothsign ? f : Math::Abs(f);
			return f * absmax * 2.f;
		}
		inline Color RandomColor(float absmax = 1.f){
			return Color(rng.Float(), rng.Float(), rng.Float()) * absmax;
		}
		inline Vector3 RandomVector3(float absmin = 1e-6f, float absmax = 1e2f, bool bothsign = true){
			return Vector3(
				RandomFloat(absmin, absmax, bothsign),
				RandomFloat(absmin, absmax, bothsign),
				RandomFloat(absmin, absmax, bothsign));
		}
		inline Vector4 RandomVector4(float absmin = 1e-6f, float absmax = 1e2f, bool bothsign = true){
			return Vector4(
				RandomFloat(absmin, absmax, bothsign),
				RandomFloat(absmin, absmax, bothsign),
				RandomFloat(absmin, absmax, bothsign),
				RandomFloat(absmin, absmax, bothsign));
		}
		inline Ray RandomRay(float lengthmax = 1e2f){
			Vector3 origin = RandomVector3();
			Vector3 dir = RandomVector3(0.1f, lengthmax);
			return Ray(origin, dir);
		}
		inline Matrix4x4 RandomMatrix(float absmax = 10.f){
			return Matrix4x4(
				RandomVector4(0, absmax, true),
				RandomVector4(0, absmax, true),
				RandomVector4(0, absmax, true),
				RandomVector4(0, absmax, true)
				);
		}

	}
}