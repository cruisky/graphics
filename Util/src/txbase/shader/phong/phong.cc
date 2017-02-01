#include "txbase/stdafx.h"
#include "txbase/shader/phong.h"

namespace TX {
	using namespace GL;
	void Phong::CompileProgram() {
		// compile the program
		Shader vs(GL_VERTEX_SHADER, VertShaderSrc.c_str());
		Shader fs(GL_FRAGMENT_SHADER, FragShaderSrc.c_str());
		Attach(vs);
		Attach(fs);
		Link();
		Detach(vs);
		Detach(fs);
	}

	void Phong::SetMaterial(
		const Color& ambient,
		const Color& diffuse,
		const Color& specular,
		float shininess) const {
		SetUniform("material.ambient", ambient);
		SetUniform("material.diffuse", diffuse);
		SetUniform("material.specular", specular);
		SetUniform("material.shininess", shininess);
	}

	void Phong::SetLight(const LightSource& light) const {
		SetUniform("light0.ambient", light.ambient);
		SetUniform("light0.diffuse", light.diffuse);
		SetUniform("light0.specular", light.specular);
		SetUniform("light0.position", light.position);
		SetUniform("light0.spotDirection", light.spotDirection);
		SetUniform("light0.spotExponent", light.spotExponent);
		SetUniform("light0.spotCutoff", light.spotCutoff);
		SetUniform("light0.constantAttenuation", light.constantAttenuation);
		SetUniform("light0.linearAttenuation", light.linearAttenuation);
		SetUniform("light0.quadraticAttenuation", light.quadraticAttenuation);
	}
}
