#include "stdafx.h"

namespace TX
{
	namespace Tests
	{
		TEST_CLASS(RayTests){
		public:
			TEST_METHOD(DynamicAllocTest){
				Ray *rheap = new Ray();
				Vector3 origin, dir;
				float tmin, tmax;
				repeat(i, 20){
					origin = RandomVector3();
					dir = RandomVector3().Normalize();
					tmin = RandomFloat();
					tmax = RandomFloat();
					rheap->origin = origin;
					rheap->dir = dir;
					rheap->t_min = tmin;
					rheap->t_max = tmax;
					Ray rstack(origin, dir, tmax, tmin);
					Assertions::AreClose(origin, rheap->origin);
					Assertions::AreClose(dir, rheap->dir);
					Assertions::AreClose(tmin, rheap->t_min);
					Assertions::AreClose(tmax, rheap->t_max);
					Assert::IsTrue(rstack.origin == rheap->origin);
					Assertions::AreClose(rstack.dir, rheap->dir);
					Assert::AreEqual(rstack.t_min, rheap->t_min);
					Assert::AreEqual(rstack.t_max, rheap->t_max);
				}
			}

			TEST_METHOD(SetSegmentTest){
				Ray ray;
				float eps1, eps2;
				Vector3 origin, dest, dir_exp;
				repeat(i, 20){
					origin = RandomVector3();
					do{ 
						dest = RandomVector3(); 
						dir_exp = dest - origin;
					} while (dir_exp.Length() < 1e-2f);

					if (i < 10){
						// default epsilons
						eps1 = eps2 = Ray::EPSILON;
						ray.SetSegment(origin, dest);
					}
					else {
						// specify epsilons
						eps1 = RandomFloat(false);
						eps2 = RandomFloat(false);
						ray.SetSegment(origin, dest, eps2, eps1);
					}

					Assertions::AreClose(origin, ray.origin);					// origin
					Assertions::AreClose(dir_exp/dir_exp.Length(), ray.dir);	// dir
					Assertions::AreClose(eps1, ray.t_min);						// t_min
					Assertions::AreClose(dir_exp.Length() - eps2, ray.t_max);	// t_max
				}
			}
		};
	}
}