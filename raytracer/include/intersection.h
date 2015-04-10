#pragma once

#include "fwddecl.h"
#include "rayhit.h"

namespace Cruisky{
	namespace RayTracer{
		class Intersection{
		public:
			Intersection() : material(nullptr){}
			~Intersection(){}

		public:
			RayHit hit;
			Material* material;
		};
	}
}