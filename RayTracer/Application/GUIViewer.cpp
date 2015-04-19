#include "stdafx.h"
#include "GUIViewer.h"

#include "Core/Film.h"
#include "Core/Scene.h"
#include "Core/Camera.h"
#include "Core/Renderer.h"

namespace Cruisky{
	namespace RayTracer{
		GUIViewer::GUIViewer(shared_ptr<Scene> scene, shared_ptr<Camera> camera, shared_ptr<Film> film) : 
			Application(), scene_(scene), camera_(camera), film_(film), renderer_(new Renderer){}
		
		void GUIViewer::Start(){
			AttemptRenderScene();
		}

		void GUIViewer::Config(){
			strcpy_s(config.title, "RayTracer");
			config.width = camera_->Width();
			config.height = camera_->Height();
			config.fixsize = true;
		}

		bool GUIViewer::Render(){
			glDrawPixels(config.width, config.height, GL_RGBA, GL_FLOAT, (float *)film_->Pixels());
			return true;
		}

		void GUIViewer::AttemptRenderScene(){
			if (!rendering.load()){
				rendering.store(true);
				film_->Resize(camera_->Width(), camera_->Height());
				task_ = std::async(std::launch::async, &GUIViewer::AsyncRenderScene, this);
			}
		}

		void GUIViewer::AsyncRenderScene(){
			renderer_->Render(scene_.get(), camera_.get(), film_.get());
			rendering.store(false);
		}
	}
}