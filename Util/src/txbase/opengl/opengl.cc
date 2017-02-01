#include "txbase/stdafx.h"
#include "opengl.h"
#include "txbase/math/vector.h"
#include "txbase/math/matrix.h"
#include "txbase/sys/memory.h"

namespace TX
{
	namespace GL
	{
		void SetUniform(GLuint loc, const Vec2& v) { glUniform2fv(loc, 1, v.data); }
		void SetUniform(GLuint loc, const Vec3& v) { glUniform3fv(loc, 1, v.data); }
		void SetUniform(GLuint loc, const Vec4& v) { glUniform4fv(loc, 1, v.data); }
		void SetUniform(GLuint loc, const Color& v) { glUniform4fv(loc, 1, v.v); }
		void SetUniform(GLuint loc, const Matrix3x3& v, bool transpose) { glUniformMatrix3fv(loc, 1, transpose, v); }
		void SetUniform(GLuint loc, const Matrix4x4& v, bool transpose) { glUniformMatrix4fv(loc, 1, transpose, v); }

		Shader::Shader(GLenum type, const char *src){
			id = glCreateShader(type);
			glShaderSource(id, 1, &src, NULL);
			glCompileShader(id);
			GLint status = GL_FALSE;
			glGetShaderiv(id, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE){
				std::clog << GetLog() << std::endl;
				throw "Failed to compile shader";
			}
		}
		Shader::~Shader(){
			glDeleteShader(id);
		}
		std::string Shader::GetLog(){
			GLint length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			GLchar *buffer = new GLchar[length];
			glGetShaderInfoLog(id, length, NULL, buffer);
			std::string log(buffer);
			delete[] buffer;
			return log;
		}

		Program::Program(){
			id = glCreateProgram();
		}
		Program::~Program(){
			glDeleteProgram(id);
		}
		void Program::Use() const { glUseProgram(id); }
		void Program::Attach(const Shader& shader){
			glAttachShader(id, shader);
		}
		void Program::Detach(const Shader& shader){
			glDetachShader(id, shader);
		}
		void Program::Link(){
			glLinkProgram(id);
			GLint status = GL_FALSE;
			glGetProgramiv(id, GL_LINK_STATUS, &status);
			if (status == GL_FALSE){
				std::clog << GetLog() << std::endl;
				throw "Failed to link program";
			}
			glValidateProgram(id);
			status = GL_FALSE;
			glGetProgramiv(id, GL_VALIDATE_STATUS, &status);
			if (status == GL_FALSE){
				std::clog << GetLog() << std::endl;
				throw "Failed to validate program";
			}
		}

		void Program::BindAttribLoc(const char *name, GLuint index){ glBindAttribLocation(id, index, name); }
		std::string Program::GetLog(){
			GLint length;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
			GLchar *buffer = new GLchar[length];
			glGetProgramInfoLog(id, length, NULL, buffer);
			std::string log(buffer);
			delete[] buffer;
			return log;
		}

		void Mesh::Upload(const TX::Mesh& mesh){
			if (mesh.vertices.size() > 0){
				vertices.Data(mesh.vertices.size() * sizeof(mesh.vertices[0]), mesh.vertices.data());
			}
			if (mesh.normals.size() > 0){
				normals.Data(mesh.normals.size() * sizeof(mesh.normals[0]), mesh.normals.data());
			}
			if (mesh.indices.size() > 0){
				indices.Data(mesh.indices.size() * sizeof(mesh.indices[0]), mesh.indices.data());
			}
		}

		void Mesh::Draw() const {
			vertices.Bind();
			glVertexAttribPointer(ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			normals.Bind();
			glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			indices.Bind();
			int size; glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			glDrawElements(GL_TRIANGLES, size / sizeof(uint), GL_UNSIGNED_INT, 0);
		}
	}
}
