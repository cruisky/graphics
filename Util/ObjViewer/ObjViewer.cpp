#include "UtilStdAfx.h"
#include "ObjViewer/ObjViewer.h"
#include "Graphics/Camera.h"
#include "System/Tools.h"
#include "Graphics/Scene.h"
#include "Graphics/Primitive.h"

namespace TX {

	ObjViewer::ObjViewer(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene) : camera(camera), scene(scene) {
		program.Compile();

		// load object data
		std::vector<std::shared_ptr<Primitive>> primitives;
		scene->GetPrimitives(primitives);

		// create buffers for each mesh object
		objs.resize(primitives.size());
		for (uint i = 0; i < primitives.size(); i++) {
			objs[i].prim = primitives[i];
			objs[i].mesh.Upload(*primitives[i]->GetMesh());
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
		program.SetMVP(
			Matrix4x4::IDENTITY,
			camera->transform.WorldToLocalMatrix(),
			camera->CameraToViewport(),
			camera->transform.LocalToWorldMatrix(),
			Matrix3x3::IDENTITY);

		// Vertex, Normal
		glEnableVertexAttribArray(GL::ATTRIB_POS);
		glEnableVertexAttribArray(GL::ATTRIB_NORMAL);

		// Light
		program.SetLight(lightSource);

		for (auto& obj : objs) {
			const BSDF *bsdf = obj.prim->GetBSDF();
			program.SetMaterial(
				bsdf->GetAmbient(),
				bsdf->GetDiffuse(),
				bsdf->GetSpecular(),
				bsdf->GetShininess());

			obj.mesh.Draw();
		}

		glPopClientAttrib();
		glPopAttrib();
	}
}
