#pragma once
#include "Graphics/Obj.h"
#include "Graphics/GLUtils.h"
#include "Shader/Phong.h"

namespace TX {
	using namespace UI;
	class ObjViewer {
	public:
		struct Obj {
			Obj(const Primitive& prim) :
				prim(prim) {
			}
			GL::Mesh mesh;
			const Primitive& prim;
		};
	public:
		Phong::LightSource lightSource;
	public:
		ObjViewer(const Camera& camera, const Scene& scene);
	public:
		void Render(const Primitive *selected);
	private:
		Phong											programPhong;
		ShaderProgram									programSingleColor;
		std::vector<Obj>								objs;
		const Camera&									camera;
		const Scene&									scene;
	};
}
