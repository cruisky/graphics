#include "TXBase/Shape/Obj.h"
#include "TXBase/OpenGL/Application.h"
#include "TXBase/OpenGL/GLUtils.h"
#include "TXBase/Scene/Camera.h"
#include "TXBase/Scene/CameraController.h"
#include "TXBase/System/Tools.h"

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
		std::unique_ptr<CameraController>				camera_ctrl;
		bool											mouse_navigation;
		Input											input;
		const float										CAM_ROT_SPEED = 0.005f;
		const float										CAM_ROLL_SPEED = 1.f;
		const float										CAM_MOV_SPEED = 2.f;
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

			// set up camera and controller
			camera = std::make_shared<Camera>(config.windowSize.x, config.windowSize.y);
			camera->transform.Translate(0.f, 0.8f, 2.f);
			camera_ctrl = std::make_unique<CameraController>(camera->transform);

			// load object data
			ObjLoader::Load(shapes, materials, "teapot.obj", "./");

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

		void OnKey(KeyCode code, KeyState state, Modifiers modifiers){
			if (state == KeyState::DOWN || state == KeyState::HOLD) {
				Vec2 horizontal; float roll = 0; float vertical = 0;
				switch (code) {
				case KeyCode::W: horizontal.y = +1; break;
				case KeyCode::S: horizontal.y = -1; break;
				case KeyCode::A: horizontal.x = -1; break;
				case KeyCode::D: horizontal.x = +1; break;
				case KeyCode::Q: roll = -1; break;
				case KeyCode::E: roll = +1; break;
				case KeyCode::SPACE: vertical = +1; break;
				case KeyCode::LEFT_SHIFT: vertical = -1; break;
				}
				float delta = GetDeltaTime();
				camera_ctrl->HorizontalMove(delta, horizontal * CAM_MOV_SPEED);
				camera_ctrl->Roll(delta, roll * CAM_ROLL_SPEED);
				camera_ctrl->VerticalMove(delta, vertical * CAM_MOV_SPEED);
			}
		}

		void OnMouseMove(float x, float y) {
			input.SetCursor(x, y);
			if (mouse_navigation) {
				Vec2 mov = input.GetCursorMovement();
				mov.x *= -1;
				camera_ctrl->Turn(mov * CAM_ROT_SPEED);
			}
		}

		bool Render(){
			mouse_navigation = Get(MouseButton::RIGHT) == MouseButtonState::DOWN;

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
