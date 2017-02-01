#pragma once
#include "TXBase/Math/Transform.h"

namespace TX{
	class Scene;

	class SceneObject {
	public:
		Scene *scene;
	};

	class DynamicSceneObject : public SceneObject {
	public:
		Transform transform;
	};
}
