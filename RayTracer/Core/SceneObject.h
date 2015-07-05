#pragma once
#include "fwddecl.h"

#include "Transform.h"

namespace TX{
	namespace RayTracer{
		class SceneObject : public Align16{
		public:
			Transform transform;
		};
	}
}