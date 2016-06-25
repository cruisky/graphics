#include "UtilStdAfx.h"
#include "ObjViewer/ObjViewer.h"
#include "Graphics/Camera.h"
#include "System/Tools.h"
#include "Graphics/Scene.h"
#include "Graphics/Primitive.h"

namespace TX {
	void ObjViewer::Prim::SetMaterial(GL::Program& program) const {
		const BSDF *bsdf = prim->GetBSDF();
		program.SetUniform("material.ambient", bsdf->GetAmbient());
		program.SetUniform("material.diffuse", bsdf->GetDiffuse());
		program.SetUniform("material.specular", bsdf->GetSpecular());
		program.SetUniform("material.shininess", bsdf->GetShininess());
	}
	void ObjViewer::Prim::Draw() const {
		mesh.vertices.Bind();
		glVertexAttribPointer(ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		mesh.normals.Bind();
		glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		mesh.indices.Bind();
		int size; glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size / sizeof(uint), GL_UNSIGNED_INT, 0);
	}

	ObjViewer::ObjViewer(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene) : camera(camera), scene(scene) {
		// compile the program
		GL::Shader vs(GL_VERTEX_SHADER, VertShaderSrc.c_str());
		GL::Shader fs(GL_FRAGMENT_SHADER, FragShaderSrc.c_str());
		program.Attach(vs);
		program.Attach(fs);
		program.Link();
		program.Detach(vs);
		program.Detach(fs);

		// uniforms
		uniform[UNIFORM_M] = program.GetUniformLoc("m");
		uniform[UNIFORM_V] = program.GetUniformLoc("v");
		uniform[UNIFORM_P] = program.GetUniformLoc("p");
		uniform[UNIFORM_M_3X3_INV_TRANSP] = program.GetUniformLoc("m_3x3_inv_transp");
		uniform[UNIFORM_V_INV] = program.GetUniformLoc("v_inv");

		// bind attribute locations
		program.BindAttribLoc("position", ATTRIB_POS);
		program.BindAttribLoc("normal", ATTRIB_NORMAL);

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
		program.Use();

		// Model, View, Projection
		GL::SetUniform(uniform[UNIFORM_M], Matrix4x4::IDENTITY);
		GL::SetUniform(uniform[UNIFORM_V], camera->transform.WorldToLocalMatrix());
		GL::SetUniform(uniform[UNIFORM_P], camera->CameraToViewport());
		GL::SetUniform(uniform[UNIFORM_V_INV], camera->transform.LocalToWorldMatrix());
		GL::SetUniform(uniform[UNIFORM_M_3X3_INV_TRANSP], Matrix3x3::IDENTITY);

		// Vertex, Normal
		glEnableVertexAttribArray(ATTRIB_POS);
		glEnableVertexAttribArray(ATTRIB_NORMAL);

		// Light
		UploadLight();

		for (auto& prim : prims) {
			prim.SetMaterial(program);
			prim.Draw();
		}

		glPopClientAttrib();
		glPopAttrib();
	}

	void ObjViewer::UploadLight() {
		program.SetUniform("light0.ambient", lightSource.ambient);
		program.SetUniform("light0.diffuse", lightSource.diffuse);
		program.SetUniform("light0.specular", lightSource.specular);
		program.SetUniform("light0.position", lightSource.position);
		program.SetUniform("light0.spotDirection", lightSource.spotDirection);
		program.SetUniform("light0.spotExponent", lightSource.spotExponent);
		program.SetUniform("light0.spotCutoff", lightSource.spotCutoff);
		program.SetUniform("light0.constantAttenuation", lightSource.constantAttenuation);
		program.SetUniform("light0.linearAttenuation", lightSource.linearAttenuation);
		program.SetUniform("light0.quadraticAttenuation", lightSource.quadraticAttenuation);
	}
}
