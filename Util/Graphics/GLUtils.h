#pragma once

#include <GL/glew.h>
#include "Util.h"

namespace TX
{
	namespace GL
	{
		class Object {
		public:
			GLuint id = 0;
			inline operator GLuint() { return id; }
		};

		template<GLenum Target>
		class Buffer : public Object {
		public:
			Buffer(){ glGenBuffers(1, &id); }
			~Buffer(){ glDeleteBuffers(1, &id); }
			void Data(GLsizeiptr size, const void *data){
				Bind();
				glBufferData(Target, size, data, GL_STATIC_DRAW);
			}
			void Bind(){ glBindBuffer(Target, id); }
			static void Unbind(){ glBindBuffer(Target, 0); }
		};
		typedef Buffer<GL_ARRAY_BUFFER> VertexBuffer;
		typedef Buffer<GL_ELEMENT_ARRAY_BUFFER> IndexBuffer;
		typedef Buffer<GL_UNIFORM_BUFFER> UniformBuffer;
		typedef Buffer<GL_TEXTURE_BUFFER> TextureBuffer;

		class Shader : public Object {
		public:
			Shader(GLenum type, const char *source);
			~Shader();

			std::string GetLog();
		};

		class Program : public Object {
		public:
			Program();
			~Program();
			void Use();
			void Attach(Shader& shader);
			void Detach(Shader& shader);
			GLuint GetUniformLoc(const char *name);
			void SetUniform(GLuint loc, int v);
			void SetUniform(GLuint loc, float v);
			void SetUniform(GLuint loc, const Vector2& v);
			void SetUniform(GLuint loc, const Vector3& v);
			void SetUniform(GLuint loc, const Vector4& v);
			void SetUniform(GLuint loc, const Matrix3x3& v, bool transpose);
			void SetUniform(GLuint loc, const Matrix4x4& v, bool transpose);
			void BindAttribLoc(const char *name, GLuint index);
			void Link();
			std::string GetLog();
		};
	}
}