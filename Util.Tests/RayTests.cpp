#include "stdafx.h"

namespace TX
{
	namespace Tests
	{
		TEST_CLASS(RayTests){
		public:
			TEST_METHOD(SetSegmentTest){
				Ray ray;
				float eps1, eps2;
				Vector3 origin, dest, dir_exp;
				repeat(i, 20){
					origin = RandomVector();
					do{ 
						dest = RandomVector(); 
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