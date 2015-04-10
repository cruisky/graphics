#pragma once

#include "fwddecl.h"
#include "rayhit.h"

namespace Cruisky{
	namespace RayTracer{
		class Intersection{
		public:
			Intersection(){}

		public:
			RayHit hit;
			const Primitive *prim;
		};
	}
}