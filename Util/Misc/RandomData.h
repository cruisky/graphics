#pragma once

#include "Math/Random.h"
#include "Math/Color.h"
#include "Math/Ray.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "SSE/SSE.h"

namespace TX
{
	namespace Tests
	{
		extern RNG rng;

		/* excluding the max */
		inline int RandomInt(int min = 0, int max = Math::MAX) {
			return Math::Clamp(min + int(rng.Float() * (max - min)), min, max - 1);
		}
		/* excluding the max */
		inline uint RandomUint(uint min = 0, uint max = Math::MAX) {
			return Math::Clamp(min + uint(rng.Float() * (max - min)), min, max - 1);
		}
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

		inline SSE::V4Float RandomV4Float(float absmin = 1e-6f, float absmax = 1e2f, bool bothsign = true) {
			return SSE::V4Float(
				RandomFloat(absmin, absmax, bothsign),
				RandomFloat(absmin, absmax, bothsign),
				RandomFloat(absmin, absmax, bothsign),
				RandomFloat(absmin, absmax, bothsign));
		}
		inline SSE::V4Int RandomV4Int(int min = 0, int max = Math::MAX) {
			return SSE::V4Int(
				RandomInt(min, max),
				RandomInt(min, max),
				RandomInt(min, max),
				RandomInt(min, max));
		}
		inline SSE::V4Bool RandomV4Bool() {
			return SSE::V4Bool(
				RandomFloat(0.f, 1.f, true) < 0.f,
				RandomFloat(0.f, 1.f, true) < 0.f,
				RandomFloat(0.f, 1.f, true) < 0.f,
				RandomFloat(0.f, 1.f, true) < 0.f);
		}
	}
}
