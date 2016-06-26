#pragma once
#include "Graphics/Obj.h"
#include "Graphics/GLUtils.h"
#include "Shader/Phong.h"

namespace TX {
	using namespace UI;
	class ObjViewer {
	public:
		struct Obj {
			GL::Mesh mesh;
			std::shared_ptr<Primitive> prim;
		};
	public:
		Phong::LightSource lightSource;
	public:
		ObjViewer(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene);
	public:
		void Render();
	private:
		Phong											program;
		std::vector<Obj>								objs;
		std::shared_ptr<Camera>							camera;
		std::shared_ptr<Scene>							scene;
	};
}
