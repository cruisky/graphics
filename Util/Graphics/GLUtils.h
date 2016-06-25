#pragma once

#include "Util.h"
#include <GL/glew.h>
#include "Graphics/Obj.h"
#include "System/Memory.h"

namespace TX
{
	namespace GL
	{
		inline void SetUniform(GLuint loc, int v) { glUniform1i(loc, v); }
		inline void SetUniform(GLuint loc, float v) { glUniform1f(loc, v); }
		void SetUniform(GLuint loc, const Vec2& v);
		void SetUniform(GLuint loc, const Vec3& v);
		void SetUniform(GLuint loc, const Vec4& v);
		void SetUniform(GLuint loc, const Color& v);
		void SetUniform(GLuint loc, const Matrix3x3& v, bool transpose = true);
		void SetUniform(GLuint loc, const Matrix4x4& v, bool transpose = true);

		class Object {
		public:
			GLuint id;
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
			void Bind() const { glBindBuffer(Target, id); }
			static void Unbind() { glBindBuffer(Target, 0); }
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

		class Program : public Object, public NonCopyable {
		public:
			Program();
			~Program();
			void Use() const;
			void Attach(Shader& shader);
			void Detach(Shader& shader);
			GLuint GetUniformLoc(const char *name) const;
			template<typename T>
			inline void SetUniform(const char *name, T v) { GL::SetUniform(GetUniformLoc(name), v); }
			inline void SetUniform(const char *name, const Matrix3x3& v, bool transpose) { GL::SetUniform(GetUniformLoc(name), v, transpose); }
			inline void SetUniform(const char *name, const Matrix4x4& v, bool transpose) { GL::SetUniform(GetUniformLoc(name), v, transpose); }

			void BindAttribLoc(const char *name, GLuint index);
			void Link();
			std::string GetLog();
		};

		class Mesh {
		public:
			Buffer<GL_ARRAY_BUFFER> vertices;
			Buffer<GL_ARRAY_BUFFER> normals;
			Buffer<GL_ELEMENT_ARRAY_BUFFER> indices;
		public:
			Mesh(){}
			void Upload(const TX::Mesh& mesh);
		};
	}
}
