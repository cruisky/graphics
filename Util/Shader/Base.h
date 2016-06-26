#pragma once

#include "Graphics/GLUtils.h"

namespace TX {
	class ShaderProgram : public GL::Program {
	private:
		enum Uniform {
			MVP_M,
			MVP_V,
			MVP_P,
			MVP_M_3X3_INV_TR,
			MVP_V_INV,
			MVP_COUNT
		};
	public:
		void Compile();
		void Compile(const std::initializer_list<GL::Shader>& shaders);
		inline void SetMVP(
			const Matrix4x4& m,
			const Matrix4x4& v,
			const Matrix4x4& p,
			const Matrix4x4& vInv,
			const Matrix3x3& m3x3InvTr
			) const {
			assert(compiled);
			GL::SetUniform(mvp[MVP_M], m);
			GL::SetUniform(mvp[MVP_V], v);
			GL::SetUniform(mvp[MVP_P], p);
			GL::SetUniform(mvp[MVP_V_INV], vInv);
			GL::SetUniform(mvp[MVP_M_3X3_INV_TR], m3x3InvTr);
		}
	private:
		// override this in the subclass
		virtual void CompileProgram(){}
		void PostCompile();
	protected:
		GLuint mvp[MVP_COUNT];
	private:
		bool compiled = false;
	};
}
