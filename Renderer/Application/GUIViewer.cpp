#include "stdafx.h"
#include "GUIViewer.h"

#include "Graphics/Film.h"
#include "Graphics/Scene.h"
#include "Graphics/Camera.h"

namespace TX{
	namespace UI{
		GUIViewer::GUIViewer(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera, std::shared_ptr<Film> film) :
			Application(), scene_(scene), camera_(camera), film_(film){
			// Progress monitor
			monitor_ = std::make_shared<ProgressMonitor>();
			// Renderer
			renderer_ = std::make_unique<Renderer>(RendererConfig(), scene, camera, film, monitor_);
		}

		void GUIViewer::Start(){
			progress_reporting = true;
			progress_reporter_job_ = std::thread(&GUIViewer::ProgressReporterJob, this);
			InvalidateFrame();
		}

		void GUIViewer::Config(){
			config.title = "Renderer";
			config.width = camera_->Width();
			config.height = camera_->Height();
			config.fixsize = false;
		}

		GUIViewer& GUIViewer::ConfigRenderer(RendererConfig config){
			renderer_->Config(config);
			return *this;
		}

		bool GUIViewer::Render(){
			if (renderer_->Running()) {
				glDrawPixels(config.width, config.height, GL_RGBA, GL_FLOAT, (float *)film_->Pixels());
				return true;
			}
			return false;
		}
		void GUIViewer::OnMouseButton(MouseButton button, MouseButtonState state, Modifiers mods) {
			Vec2 cursor;
			GetCursorPos(&cursor.x, &cursor.y);
			FlipY(&cursor.y);
			if (state == MouseButtonState::DOWN){
				switch (button){
				case MouseButton::LEFT:
					Color c = film_->Get(int(cursor.x), int(cursor.y));
					std::printf("(%3f, %3f), (%1.3f, %1.3f, %1.3f)\n", cursor.x, cursor.y, c.r, c.g, c.b);
					break;
				}
			}
		}

		void GUIViewer::OnKey(KeyCode code, KeyState state, Modifiers modifiers){
			if (state == KeyState::DOWN || state == KeyState::HOLD) {
				switch (code) {
				case KeyCode::UP: AttemptPanTiltCamera(Direction::UP); break;
				case KeyCode::DOWN: AttemptPanTiltCamera(Direction::DOWN); break;
				case KeyCode::LEFT: AttemptPanTiltCamera(Direction::LEFT); break;
				case KeyCode::RIGHT: AttemptPanTiltCamera(Direction::RIGHT); break;
				case KeyCode::W: AttemptDollyCrabCamera(Direction::UP); break;
				case KeyCode::S: AttemptDollyCrabCamera(Direction::DOWN); break;
				case KeyCode::A: AttemptDollyCrabCamera(Direction::LEFT); break;
				case KeyCode::D: AttemptDollyCrabCamera(Direction::RIGHT); break;
				case KeyCode::Q: AttemptRollCamera(false); break;
				case KeyCode::E: AttemptRollCamera(true); break;
				case KeyCode::ESCAPE: Exit(); break;
				}
			}
		}

		void GUIViewer::OnResize(){
			renderer_->Resize(config.width, config.height);
			InvalidateFrame();
		}
		void GUIViewer::OnExit(){
			progress_reporting = false;
			progress_reporter_job_.join();
		}


		void GUIViewer::AttemptDollyCrabCamera(Direction dir){
			float dist = 0.1f;
			Vec3 movement;
			switch (dir){
			case Direction::UP: movement = Vec3(0.f, 0.f, -dist); break;
			case Direction::DOWN: movement = Vec3(0.f, 0.f, dist); break;
			case Direction::LEFT: movement = Vec3(-dist, 0.f, 0.f); break;
			case Direction::RIGHT: movement = Vec3(dist, 0.f, 0.f); break;
			}
			camera_->transform.Translate(movement);
			InvalidateFrame();
		}

		void GUIViewer::AttemptPanTiltCamera(Direction dir){
			float rad = Math::ToRad(2.f);
			Vec3 axis;
			switch (dir){
			case Direction::UP: axis = Vec3::X; break;
			case Direction::DOWN: axis = -Vec3::X; break;
			case Direction::LEFT: axis = Vec3::Y; break;
			case Direction::RIGHT: axis = -Vec3::Y; break;
			}
			camera_->transform.Rotate(Quaternion::AngleAxis(rad, axis));
			InvalidateFrame();
		}

		void GUIViewer::AttemptRollCamera(bool clockwise){
			float rad = Math::ToRad(clockwise ? 10.f : -10.f);
			camera_->transform.Rotate(Quaternion::AngleAxis(rad, -Vec3::Z));
			InvalidateFrame();
		}


		void GUIViewer::InvalidateFrame(){
			camera_->transform.UpdateMatrix();
			renderer_->Abort();
			renderer_->NewTask();
			this->Refresh();
		}

		void GUIViewer::ProgressReporterJob(){
#ifndef _DEBUG
			bool prev_status = false, status;
			while (progress_reporting){
				Sleep(100);
				status = monitor_->InProgress();
				if (status || prev_status){
					system("CLS");
					std::printf("Progress:\t %2.1f %%\n", monitor_->Progress() * 100.f);
					std::printf("Remaining:\t %.1f s\n", monitor_->RemainingTime());
					if (!status) std::printf("Render Time:\t %.6f s\n", monitor_->ElapsedTime());
					prev_status = status;
				}
			}
			std::printf("Terminated at %3.3f%%. \n", monitor_->Progress() * 100.f);
#endif
		}
		void GUIViewer::FlipY(float *y) { *y = film_->Height() - *y - 1; }
		void GUIViewer::FlipX(float *x) { *x = film_->Width() - *x - 1; }
	}
}
