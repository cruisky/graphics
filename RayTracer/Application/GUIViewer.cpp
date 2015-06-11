#include "stdafx.h"
#include "GUIViewer.h"

#include "Core/Film.h"
#include "Core/Scene.h"
#include "Core/Camera.h"
#include "Core/Renderer.h"
#include "Core/Config.h"
#include "Tools.h"

namespace Cruisky{
	namespace RayTracer{
		GUIViewer::GUIViewer(shared_ptr<Scene> scene, shared_ptr<Camera> camera, shared_ptr<Film> film) :
			Application(), scene_(scene), camera_(camera), film_(film){
			 renderer_ = std::make_unique<Renderer>(RendererConfig());
			 monitor_= std::make_shared<ProgressMonitor>();
		}
		
		void GUIViewer::Start(){
			progress_reporter_job_ = std::thread(&GUIViewer::ProgressReporterJob, this);
			RenderScene();
		}

		void GUIViewer::Config(){
			strcpy_s(config.title, "RayTracer");
			config.width = camera_->Width();
			config.height = camera_->Height();
			config.fixsize = true;
		}

		GUIViewer& GUIViewer::ConfigRenderer(RendererConfig config){
			renderer_.reset(new Renderer(config));
			return *this;
		}

		bool GUIViewer::Render(){
			glDrawPixels(config.width, config.height, GL_RGBA, GL_FLOAT, (float *)film_->Pixels());
			return true;
		}

		void GUIViewer::OnMouseButton(Button button, ButtonState state, int x, int y) {
			FlipY(&y);

			if (state == ButtonState::DOWN){
				switch (button){
				case Button::LEFT:
					Color c = film_->Get(x, y);
					printf("(%3d, %3d), (%1.3f, %1.3f, %1.3f)\n", x, y, c.r, c.g, c.b); break;
				}
			}
		}

		void GUIViewer::OnKey(unsigned char c, int x, int y){
			switch (toupper(c)){
			case 'W': AttemptMoveCamera(Direction::UP); break;
			case 'S': AttemptMoveCamera(Direction::DOWN); break;
			case 'A': AttemptMoveCamera(Direction::LEFT); break;
			case 'D': AttemptMoveCamera(Direction::RIGHT); break;
			case 'Q': AttemptBarrelRollCamera(false); break;
			case 'E': AttemptBarrelRollCamera(true); break;
			case 27: Exit(); break;			// escape
			}
		}

		void GUIViewer::OnSpecialKey(KeyCode code, int x, int y){
			switch (code){
			case KeyCode::UP: AttemptPanCamera(Direction::UP); break;
			case KeyCode::DOWN: AttemptPanCamera(Direction::DOWN); break;
			case KeyCode::LEFT: AttemptPanCamera(Direction::LEFT); break;
			case KeyCode::RIGHT: AttemptPanCamera(Direction::RIGHT); break;
			}
		}

		void GUIViewer::AttemptMoveCamera(Direction dir){
			float dist = 1.f;
			Vector3 movement;
			if (!rendering.load()){
				switch (dir){
				case Direction::UP: movement = Vector3(0.f, 0.f, -dist); break;
				case Direction::DOWN: movement = Vector3(0.f, 0.f, dist); break;
				case Direction::LEFT: movement = Vector3(-dist, 0.f, 0.f); break;
				case Direction::RIGHT: movement = Vector3(dist, 0.f, 0.f); break;
				}
				camera_->transform.Translate(movement);
				RenderScene();
			}
		}

		void GUIViewer::AttemptPanCamera(Direction dir){
			float degree = 10.f;
			Vector3 axis; 
			if (!rendering.load()){
				switch (dir){
				case Direction::UP: axis = Vector3::X; break;
				case Direction::DOWN: axis = -Vector3::X; break;
				case Direction::LEFT: axis = Vector3::Y; break;
				case Direction::RIGHT: axis = -Vector3::Y; break;
				}
				camera_->transform.Rotate(degree, axis);
				RenderScene();
			}
		}

		void GUIViewer::AttemptBarrelRollCamera(bool clockwise){
			float degree = clockwise ? 10.f : -10.f;
			if (!rendering.load()){
				camera_->transform.Rotate(degree, -Vector3::Z);
				RenderScene();
			}
		}


		void GUIViewer::RenderScene(){
			assert(!rendering.load());
			rendering.store(true);
			render_task_ = std::async(std::launch::async, &GUIViewer::AsyncRenderScene, this);
		}

		void GUIViewer::ProgressReporterJob(){
			bool prev_status = false, status;
			while (true){
				Sleep(100);
				status = monitor_->InProgress();
				if (status || prev_status){
					prev_status = status;
					system("CLS");
					printf("Progress:\t %2.1f%%\n", monitor_->Progress() * 100.f);
					//printf("Elapsed:\t %.0f s\n", monitor_->ElapsedTime());
					printf("Remaining:\t %.0f s\n", monitor_->RemainingTime());
				}
			}
		}

		void GUIViewer::AsyncRenderScene(){
			if (camera_->Width() != film_->Width() || camera_->Height() != film_->Height())
				film_->Resize(camera_->Width(), camera_->Height());
			film_->Reset();
			renderer_->Render(scene_.get(), camera_.get(), film_.get(), monitor_);
			rendering.store(false);
		}

		void GUIViewer::FlipY(int *y) { *y = film_->Height() - *y - 1; }
		void GUIViewer::FlipX(int *x) { *x = film_->Width() - *x - 1; }

	}
}