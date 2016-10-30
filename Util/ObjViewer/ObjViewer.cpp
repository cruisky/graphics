#include "UtilStdAfx.h"
#include "ObjViewer/ObjViewer.h"
#include "Graphics/Camera.h"
#include "System/Tools.h"
#include "Graphics/Scene.h"
#include "Graphics/Primitive.h"

namespace TX {

	ObjViewer::ObjViewer(const Camera& camera, const Scene& scene) : camera(camera), scene(scene) {
		programPhong.Compile();
		programSingleColor.Compile({
			GL::Shader(GL_VERTEX_SHADER,
#include "Shader/Default.vs.glsl"
			),
			GL::Shader(GL_FRAGMENT_SHADER,
#include "Shader/SingleColor.fs.glsl"
			)
		});

		// load object data
		std::vector<std::shared_ptr<Primitive>> primitives;
		scene.GetPrimitives(primitives);

		// create buffers for each mesh object
		objs.reserve(primitives.size());
		for (uint i = 0; i < primitives.size(); i++) {
			objs.emplace_back(*primitives[i]);
			objs[i].mesh.Upload(*primitives[i]->GetMesh());
		}
	}

	void ObjViewer::Render(const Primitive *selected) {
		glPushAttrib(GL_ENABLE_BIT);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

		glClearColor(0.15f, 0.15f, 0.15f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		camera.transform.UpdateMatrix();

		// Vertex, Normal
		glEnableVertexAttribArray(GL::ATTRIB_POS);
		glEnableVertexAttribArray(GL::ATTRIB_NORMAL);

		programPhong.Use();

		// Model, View, Projection
		programPhong.SetMVP(
			Matrix4x4::IDENTITY,
			camera.transform.WorldToLocalMatrix(),
			camera.CameraToViewport(),
			camera.transform.LocalToWorldMatrix(),
			Matrix3x3::IDENTITY);

		// Light
		programPhong.SetLight(lightSource);

		Obj *outlinedObj = nullptr;
		for (auto& obj : objs) {
			if (&obj.prim == selected) {
				outlinedObj = &obj;
			}
			const BSDF *bsdf = obj.prim.GetBSDF();
			programPhong.SetMaterial(
				bsdf->GetAmbient(),
				bsdf->GetDiffuse(),
				bsdf->GetSpecular(),
				bsdf->GetShininess());

			obj.mesh.Draw();
		}

		if (outlinedObj) {
			glDisable(GL_DEPTH_TEST);

			programSingleColor.Use();

			// Model, View, Projection
			programSingleColor.SetMVP(
				Matrix4x4::IDENTITY,
				camera.transform.WorldToLocalMatrix(),
				camera.CameraToViewport(),
				camera.transform.LocalToWorldMatrix(),
				Matrix3x3::IDENTITY);

			// draw wireframe of selected object
			programSingleColor.SetUniform("color", Color(0, 0.6, 0.8));
			glLineWidth(1);
			glPolygonMode(GL_FRONT, GL_LINE);

			outlinedObj->mesh.Draw();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glPopClientAttrib();
		glPopAttrib();
	}
}
