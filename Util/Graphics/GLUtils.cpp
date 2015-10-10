#include "UtilStdAfx.h"
#include "GLUtils.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "System/Memory.h"

namespace TX
{
	namespace GL
	{
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
		void Program::Use(){ glUseProgram(id); }
		void Program::Attach(Shader& shader){ 
			glAttachShader(id, shader); 
		}
		void Program::Detach(Shader& shader){ 
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
		GLuint Program::GetUniformLoc(const char *name) { return glGetUniformLocation(id, name); }
		void Program::SetUniform(GLuint loc, int v) { glUniform1i(loc, v); }
		void Program::SetUniform(GLuint loc, float v) { glUniform1f(loc, v); }
		void Program::SetUniform(GLuint loc, const Vec2& v) { glUniform2f(loc, v.x, v.y); }
		void Program::SetUniform(GLuint loc, const Vec3& v) { glUniform3f(loc, v.x, v.y, v.z); }
		void Program::SetUniform(GLuint loc, const Vec4& v) { glUniform4f(loc, v.x, v.y, v.z, v.w); }
		void Program::SetUniform(GLuint loc, const Matrix3x3& v, bool transpose) { glUniformMatrix3fv(loc, 9, transpose, (const float *)(v[0])); }
		void Program::SetUniform(GLuint loc, const Matrix4x4& v, bool transpose) { glUniformMatrix4fv(loc, 16, transpose, (const float *)(v[0])); }
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

		void Mesh::Upload(const ObjMesh& mesh){
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
	}
}
