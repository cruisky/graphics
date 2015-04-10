#pragma once

#include "fwddecl.h"
#include "rayhit.h"

namespace Cruisky{
	class Intersection{
	public:
		Intersection() : material(nullptr){}
		~Intersection(){}

	public:
		RayHit hit;
		Material* material;
	};
}