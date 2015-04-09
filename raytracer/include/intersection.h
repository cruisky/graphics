#pragma once

#include "core/vector.h"
#include "material.h"

namespace Cruisky{
	class Intersection{
	public:
		Intersection() : 
			exist(false), t(1.f), material(nullptr){}
		~Intersection(){}

		Vector3 point;
		Vector3 normal;
		Material* material;
		float t;
		bool exist;
	};
}