#pragma once

#include "Graphics/Color.h"
#include "Math/Ray.h"
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
		inline Vec3 RandomVec3(float absmin = 1e-6f, float absmax = 1e2f, bool bothsign = true){
			return Vec3(
				RandomFloat(absmin, absmax, bothsign),
				RandomFloat(absmin, absmax, bothsign),
				RandomFloat(absmin, absmax, bothsign));
		}
		inline Vec4 RandomVec4(float absmin = 1e-6f, float absmax = 1e2f, bool bothsign = true){
			return Vec4(
				RandomFloat(absmin, absmax, bothsign),
				RandomFloat(absmin, absmax, bothsign),
				RandomFloat(absmin, absmax, bothsign),
				RandomFloat(absmin, absmax, bothsign));
		}
		inline Ray RandomRay(float lengthmax = 1e2f){
			Vec3 origin = RandomVec3();
			Vec3 dir = RandomVec3(0.1f, lengthmax);
			return Ray(origin, dir);
		}
		inline Matrix4x4 RandomMatrix(float absmax = 10.f){
			return Matrix4x4(
				RandomVec4(0, absmax, true),
				RandomVec4(0, absmax, true),
				RandomVec4(0, absmax, true),
				RandomVec4(0, absmax, true)
				);
		}

	}
}