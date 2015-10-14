#include "Graphics/Obj.h"
#include "Graphics/Camera.h"
#include "System/Application.h"
#include "System/Tools.h"
#include "Graphics/GLUtils.h"

#include <memory>

namespace TX
{
	using namespace UI;
	class ObjViewer : public Application {
	private:
		enum Attribute{
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
		std::vector<ObjShape>							shapes;
		std::vector<ObjMaterial>						materials;
		std::shared_ptr<Camera>							camera;
	protected:
		void Start(){
			// compile the program
			GL::Shader vs(GL_VERTEX_SHADER, ReadAllLines("phong.vs.glsl").c_str());
			GL::Shader fs(GL_FRAGMENT_SHADER, ReadAllLines("phong.fs.glsl").c_str());
			program = std::make_shared<GL::Program>();
			program->Attach(vs);
			program->Attach(fs);
			program->Link();
			program->Detach(vs);
			program->Detach(fs);

			// uniforms
			uniform[UNIFORM_M] = program->GetUniformLoc("m");
			uniform[UNIFORM_V] = program->GetUniformLoc("v");
			uniform[UNIFORM_P] = program->GetUniformLoc("p");
			uniform[UNIFORM_M_3X3_INV_TRANSP] = program->GetUniformLoc("m_3x3_inv_transp");
			uniform[UNIFORM_V_INV] = program->GetUniformLoc("v_inv");

			// bind attribute locations
			program->BindAttribLoc("position", ATTRIB_POS);
			program->BindAttribLoc("normal", ATTRIB_NORMAL);

			// set up camera
			camera = std::make_shared<Camera>(config.width, config.height);
			camera->transform.Translate(0.f, 0.8f, 2.f);

			// load object data
			LoadObj(shapes, materials, "teapot.obj", "./");

			// create buffers for each mesh object
			meshes.resize(shapes.size());
			for (uint i = 0; i < shapes.size(); i++){
				meshes[i].Upload(shapes[i].mesh);
			}

			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
		}
		bool Render(){
			static const GLfloat gray[] = { 0.15f, 0.15f, 0.15f, 1.f };
			glClearBufferfv(GL_COLOR, 0, gray);
			static const GLfloat one = 1.0f;
			glClearBufferfv(GL_DEPTH, 0, &one);

			camera->transform.UpdateMatrix();
			program->Use();

			glUniformMatrix4fv(uniform[UNIFORM_M], 1, GL_TRUE, Matrix4x4::IDENTITY);
			glUniformMatrix4fv(uniform[UNIFORM_V], 1, GL_TRUE, camera->transform.WorldToLocalMatrix());
			glUniformMatrix4fv(uniform[UNIFORM_P], 1, GL_TRUE, camera->CameraToViewport());
			glUniformMatrix4fv(uniform[UNIFORM_V_INV], 1, GL_TRUE, camera->transform.LocalToWorldMatrix());
			glUniformMatrix3fv(uniform[UNIFORM_M_3X3_INV_TRANSP], 1, GL_TRUE, Matrix3x3::IDENTITY);

			glEnableVertexAttribArray(ATTRIB_POS);
			glEnableVertexAttribArray(ATTRIB_NORMAL);

			for (auto& mesh : meshes){
				mesh.vertices.Bind();
				glVertexAttribPointer(ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, 0, NULL);

				mesh.normals.Bind();
				glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

				mesh.indices.Bind();
				int size; glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
				glDrawElements(GL_TRIANGLES, size/sizeof(uint), GL_UNSIGNED_INT, 0);
			}

			return true;
		}
	};
}

int main(){
	TX::ObjViewer().Run();
	return 0;
}
