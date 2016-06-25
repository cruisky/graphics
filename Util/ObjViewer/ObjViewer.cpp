#include "UtilStdAfx.h"
#include "ObjViewer/ObjViewer.h"
#include "Graphics/Camera.h"
#include "System/Tools.h"
#include "Graphics/Scene.h"
#include "Graphics/Primitive.h"

namespace TX {
	using namespace UI;
	ObjViewer::ObjViewer(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene) : camera(camera), scene(scene) {
		// compile the program
		GL::Shader vs(GL_VERTEX_SHADER, VertShaderSrc.c_str());
		GL::Shader fs(GL_FRAGMENT_SHADER, FragShaderSrc.c_str());
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

		// load object data
		std::vector<std::shared_ptr<Primitive>> primitives;
		scene->GetPrimitives(primitives);

		// create buffers for each mesh object
		meshes.resize(primitives.size());
		for (uint i = 0; i < primitives.size(); i++) {
			meshes[i].Upload(*primitives[i]->GetMesh());
		}
	}

	void ObjViewer::Render() {
		glPushAttrib(GL_ENABLE_BIT);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

		static const GLfloat gray[] = { 0.15f, 0.15f, 0.15f, 1.f };
		glClearBufferfv(GL_COLOR, 0, gray);
		static const GLfloat one = 1.0f;
		glClearBufferfv(GL_DEPTH, 0, &one);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		camera->transform.UpdateMatrix();
		program->Use();

		glUniformMatrix4fv(uniform[UNIFORM_M], 1, GL_TRUE, Matrix4x4::IDENTITY);
		glUniformMatrix4fv(uniform[UNIFORM_V], 1, GL_TRUE, camera->transform.WorldToLocalMatrix());
		glUniformMatrix4fv(uniform[UNIFORM_P], 1, GL_TRUE, camera->CameraToViewport());
		glUniformMatrix4fv(uniform[UNIFORM_V_INV], 1, GL_TRUE, camera->transform.LocalToWorldMatrix());
		glUniformMatrix3fv(uniform[UNIFORM_M_3X3_INV_TRANSP], 1, GL_TRUE, Matrix3x3::IDENTITY);

		glEnableVertexAttribArray(ATTRIB_POS);
		glEnableVertexAttribArray(ATTRIB_NORMAL);

		for (auto& mesh : meshes) {
			mesh.vertices.Bind();
			glVertexAttribPointer(ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			mesh.normals.Bind();
			glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			mesh.indices.Bind();
			int size; glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			glDrawElements(GL_TRIANGLES, size / sizeof(uint), GL_UNSIGNED_INT, 0);
		}

		glPopClientAttrib();
		glPopAttrib();
	}
}
