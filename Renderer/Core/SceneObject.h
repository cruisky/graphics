#pragma once
#include "txbase/math/transform.h"

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
