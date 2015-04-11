#pragma once

#include "fwddecl.h"
#include "Rayhit.h"

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