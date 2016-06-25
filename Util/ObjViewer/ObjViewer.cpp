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
		prims.resize(primitives.size());
		for (uint i = 0; i < primitives.size(); i++) {
			prims[i].prim = primitives[i];
			prims[i].mesh.Upload(*primitives[i]->GetMesh());
		}
	}

	void ObjViewer::Render() {
		glPushAttrib(GL_ENABLE_BIT);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

		glClearColor(0.15f, 0.15f, 0.15f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		camera->transform.UpdateMatrix();
		program->Use();

		// Model, View, Projection
		glUniformMatrix4fv(uniform[UNIFORM_M], 1, GL_TRUE, Matrix4x4::IDENTITY);
		glUniformMatrix4fv(uniform[UNIFORM_V], 1, GL_TRUE, camera->transform.WorldToLocalMatrix());
		glUniformMatrix4fv(uniform[UNIFORM_P], 1, GL_TRUE, camera->CameraToViewport());
		glUniformMatrix4fv(uniform[UNIFORM_V_INV], 1, GL_TRUE, camera->transform.LocalToWorldMatrix());
		glUniformMatrix3fv(uniform[UNIFORM_M_3X3_INV_TRANSP], 1, GL_TRUE, Matrix3x3::IDENTITY);

		// Vertex, Normal
		glEnableVertexAttribArray(ATTRIB_POS);
		glEnableVertexAttribArray(ATTRIB_NORMAL);

		// Light
		UploadLight();

		for (auto& prim : prims) {
			const BSDF *bsdf = prim.prim->GetBSDF();
			glUniform4fv(program->GetUniformLoc("material.ambient"), 1, (float *)&bsdf->GetAmbient());
			glUniform4fv(program->GetUniformLoc("material.diffuse"), 1, (float *)&bsdf->GetDiffuse());
			glUniform4fv(program->GetUniformLoc("material.specular"), 1, (float *)&bsdf->GetSpecular());
			glUniform1f(program->GetUniformLoc("material.shininess"), bsdf->GetShininess());

			prim.mesh.vertices.Bind();
			glVertexAttribPointer(ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			prim.mesh.normals.Bind();
			glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			prim.mesh.indices.Bind();
			int size; glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			glDrawElements(GL_TRIANGLES, size / sizeof(uint), GL_UNSIGNED_INT, 0);
		}

		glPopClientAttrib();
		glPopAttrib();
	}

	void ObjViewer::UploadLight() {

		glUniform4fv(program->GetUniformLoc("light0.ambient"), 1, (float*)&lightSource.ambient);
		glUniform4fv(program->GetUniformLoc("light0.diffuse"), 1, (float*)&lightSource.diffuse);
		glUniform4fv(program->GetUniformLoc("light0.specular"), 1, (float*)&lightSource.specular);
		glUniform4fv(program->GetUniformLoc("light0.position"), 1, (float*)&lightSource.position);
		glUniform3fv(program->GetUniformLoc("light0.spotDirection"), 1, (float *)&lightSource.spotDirection);
		glUniform1f(program->GetUniformLoc("light0.spotExponent"), lightSource.spotExponent);
		glUniform1f(program->GetUniformLoc("light0.spotCutoff"), lightSource.spotCutoff);
		glUniform1f(program->GetUniformLoc("light0.constantAttenuation"), lightSource.constantAttenuation);
		glUniform1f(program->GetUniformLoc("light0.linearAttenuation"), lightSource.linearAttenuation);
		glUniform1f(program->GetUniformLoc("light0.quadraticAttenuation"), lightSource.quadraticAttenuation);
	}
}
