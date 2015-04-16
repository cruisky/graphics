#pragma once
#include "fwddecl.h"

#include "Vector.h"

namespace Cruisky{
	namespace RayTracer{

		// Basic info of a intersection
		struct Intersection{
		public:
			Intersection(){}
		public:
			float dist;
			const Primitive *prim;
		};

		struct LocalGeo : Intersection {
		public:
			LocalGeo(){}
		public:
			const BSDF *bsdf;
			Vector3 point;
			Vector3 normal;			// from surface
			//Vector3 normal;		// from normal map
		};
	}
}