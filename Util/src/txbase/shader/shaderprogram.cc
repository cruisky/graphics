#include "txbase/stdafx.h"
#include "txbase/shader/base.h"

namespace TX {
	void ShaderProgram::Compile() {
		CompileProgram();
		PostCompile();
	}

	void ShaderProgram::Compile(const std::initializer_list<GL::Shader>& shaders) {
		for (auto& s : shaders)
			Attach(s);
		Link();
		for (auto& s : shaders)
			Detach(s);
		PostCompile();
	}

	void ShaderProgram::PostCompile() {
		compiled = true;

		// standard mvp matrices
		mvp[MVP_M] = GetUniformLoc("tx_M");
		mvp[MVP_V] = GetUniformLoc("tx_V");
		mvp[MVP_P] = GetUniformLoc("tx_P");
		mvp[MVP_M_3X3_INV_TR] = GetUniformLoc("tx_M_3x3InvTr");
		mvp[MVP_V_INV] = GetUniformLoc("tx_V_Inv");

		// bind position/normal attribute location
		BindAttribLoc("tx_Pos", GL::ATTRIB_POS);
		BindAttribLoc("tx_Normal", GL::ATTRIB_NORMAL);
	}
}
