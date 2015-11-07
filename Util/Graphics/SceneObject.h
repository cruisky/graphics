#pragma once
#include "Math/Transform.h"

namespace TX{
	class SceneObject {
	public:
		Scene *scene;
	};

	class DynamicSceneObject : public SceneObject {
	public:
		Transform transform;
	};
}
