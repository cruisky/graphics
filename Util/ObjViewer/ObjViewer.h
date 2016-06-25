#pragma once
#include "Graphics/Obj.h"
#include "Graphics/GLUtils.h"
#include "Misc/Const.h"

namespace TX {
	using namespace UI;
	class ObjViewer {
	public:
		ObjViewer(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene);
	public:
		void Render();
	private:
		enum Attribute {
			ATTRIB_POS,
			ATTRIB_NORMAL,
			ATTRIB_COUNT
		};
		enum Uniform {
			UNIFORM_M,
			UNIFORM_V,
			UNIFORM_P,
			UNIFORM_M_3X3_INV_TRANSP,
			UNIFORM_V_INV,
			UNIFORM_COUNT
		};
		std::shared_ptr<GL::Program>					program;
		GLuint											uniform[UNIFORM_COUNT];
		std::vector<GL::Mesh>							meshes;
		std::shared_ptr<Camera>							camera;
		std::shared_ptr<Scene>							scene;

		/// <summary>
		/// Vertex shader code.
		/// </summary>
		constexpr static ConstString VertShaderSrc =
#include "phong.vs.glsl"
			;
		/// <summary>
		/// Fragment shader code.
		/// </summary>
		constexpr static ConstString FragShaderSrc =
#include "phong.fs.glsl"
			;
	};
}
