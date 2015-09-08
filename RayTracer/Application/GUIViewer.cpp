#include "stdafx.h"
#include "GUIViewer.h"

#include "Core/Film.h"
#include "Core/Scene.h"
#include "Core/Camera.h"
#include "Core/Renderer.h"
#include "Core/RendererConfig.h"
#include "System/Tools.h"

namespace TX{
	namespace RayTracer{
		GUIViewer::GUIViewer(shared_ptr<Scene> scene, shared_ptr<Film> film) :
			Application(), scene_(scene), film_(film){
			// Progress monitor
			monitor_ = std::make_shared<ProgressMonitor>();
			// Start worker threads
			ThreadScheduler::Instance()->StartAll();
			renderer_ = std::make_unique<Renderer>(RendererConfig(), scene, film, monitor_);
		}

		void GUIViewer::Start(){
			progress_reporter_job_ = std::thread(&GUIViewer::ProgressReporterJob, this);
			InvalidateFrame();
		}

		void GUIViewer::Config(){
			config.title = "RayTracer";
			config.width = scene_->camera->Width();
			config.height = scene_->camera->Height();
			config.fixsize = false;
		}

		GUIViewer& GUIViewer::ConfigRenderer(RendererConfig config){
			renderer_->Config(config);
			return *this;
		}

		bool GUIViewer::Render(){
			glDrawPixels(config.width, config.height, GL_RGBA, GL_FLOAT, (float *)film_->Pixels());
			return true;
		}

		void GUIViewer::OnMouseButton(MouseButton button, MouseButtonState state, int x, int y) {
			FlipY(&y);

			if (state == MouseButtonState::DOWN){
				switch (button){
				case MouseButton::LEFT:
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

		void GUIViewer::OnResize(){
			renderer_->Resize(config.width, config.height);
			InvalidateFrame();
		}


		void GUIViewer::AttemptMoveCamera(Direction dir){
			float dist = 0.1f;
			Vector3 movement;
			switch (dir){
			case Direction::UP: movement = Vector3(0.f, 0.f, -dist); break;
			case Direction::DOWN: movement = Vector3(0.f, 0.f, dist); break;
			case Direction::LEFT: movement = Vector3(-dist, 0.f, 0.f); break;
			case Direction::RIGHT: movement = Vector3(dist, 0.f, 0.f); break;
			}
			scene_->camera->transform.Translate(movement);
			InvalidateFrame();
		}

		void GUIViewer::AttemptPanCamera(Direction dir){
			float degree = 2.f;
			Vector3 axis;
			switch (dir){
			case Direction::UP: axis = Vector3::X; break;
			case Direction::DOWN: axis = -Vector3::X; break;
			case Direction::LEFT: axis = Vector3::Y; break;
			case Direction::RIGHT: axis = -Vector3::Y; break;
			}
			scene_->camera->transform.Rotate(degree, axis);
			InvalidateFrame();
		}

		void GUIViewer::AttemptBarrelRollCamera(bool clockwise){
			float degree = clockwise ? 10.f : -10.f;
			scene_->camera->transform.Rotate(degree, -Vector3::Z);
			InvalidateFrame();
		}


		void GUIViewer::InvalidateFrame(){
			renderer_->Abort();
			renderer_->NewTask();
		}

		void GUIViewer::ProgressReporterJob(){
			#ifndef _DEBUG
			bool prev_status = false, status;
			while (true){
				Sleep(100);
				status = monitor_->InProgress();
				if (status || prev_status){
					system("CLS");
					printf("============================================\n");
					printf("Progress:\t %2.1f %%\n", monitor_->Progress() * 100.f);
					printf("Remaining:\t %.1f s\n", monitor_->RemainingTime());
					if (!status) printf("Render Time:\t %.6f s\n", monitor_->ElapsedTime());
					prev_status = status;
				}
			}
			#endif
		}
		void GUIViewer::FlipY(int *y) { *y = film_->Height() - *y - 1; }
		void GUIViewer::FlipX(int *x) { *x = film_->Width() - *x - 1; }

	}
}
