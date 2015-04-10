#pragma once

#include "fwddecl.h"
#include "rayhit.h"

namespace Cruisky{
	namespace RayTracer{
		class Intersection{
		public:
			Intersection(){}
			~Intersection(){}

		public:
			RayHit hit;
			Primitive *prim;
		};
	}
}