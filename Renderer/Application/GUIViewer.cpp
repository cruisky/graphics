#include "stdafx.h"
#include "GUIViewer.h"

#include "Graphics/Film.h"
#include "Graphics/Scene.h"
#include "Graphics/Camera.h"
#include "Graphics/GUI.h"

namespace TX {
	namespace UI {
		GUIViewer::GUIViewer(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera, std::shared_ptr<Film> film) :
			InputHandledApplication(), scene_(scene), camera_(camera), film_(film) {
			config.title = "Renderer";
			config.windowSize.x = camera_->Width();
			config.windowSize.y = camera_->Height();
			config.fixsize = false;
			config.fps = 60;

			// Progress monitor
			monitor_ = std::make_shared<ProgressMonitor>();

			// Renderer
			renderer_ = std::make_unique<Renderer>(RendererConfig(), scene_, camera_, film_, monitor_);
		}

		void GUIViewer::Start() {
			// Previewer
			previewer_ = std::make_unique<ObjViewer>(camera_, scene_);

			InputHandledApplication::Start();
#pragma	region GUI settings
			font_.Load("../Assets/DroidSans/DroidSans.ttf", 14.f);
			GUI::Init(font_);
			windowMain_ = Rect(0.f, 0.f, 200.f, 180.f);
#pragma endregion
		}

		GUIViewer& GUIViewer::ConfigRenderer(RendererConfig config) {
			renderer_->Config(config);
			return *this;
		}

		bool GUIViewer::Render() {
			if (input.windowChanged) {
				renderer_->Resize(input.windowSize.x, input.windowSize.y);
				this->Refresh();
			}

			switch (state_) {
			case State::Preview:
				previewer_->Render();
				break;
			case State::Render:
				glDrawPixels(config.windowSize.x, config.windowSize.y, GL_RGBA, GL_FLOAT, (float *)film_->Pixels());
				break;
			}

			// let the GUI handle the input first
			OnGUI();

			Vec2 cursor = input.cursor;
			FlipY(&cursor.y);
			if (input.buttonState == MouseButtonState::DOWN) {
				switch (input.button) {
				case MouseButton::LEFT:
					Color c = film_->Get(int(cursor.x), int(cursor.y));
					std::printf("(%3f, %3f), (%1.3f, %1.3f, %1.3f)\n", cursor.x, cursor.y, c.r, c.g, c.b);
					break;
				}
			}
			if (input.keyState == KeyState::DOWN || input.keyState == KeyState::HOLD) {
				switch (input.key) {
				case KeyCode::ESCAPE:
					Exit(); break;
				}
			}

			input.Clear();
			return true;
		}

		void GUIViewer::OnExit() {}

		void GUIViewer::OnGUI() {
			bool rendering = renderer_->Running();
			std::stringstream ss; ss.precision(3);
			GUI::BeginFrame(input);

			if (rendering) {
				GUI::BeginWindow("Rendering...", windowMain_);
				{
					float prog = monitor_->Progress();
					ss << "Progress: " << prog * 100.f << '%';
					GUI::ProgressBar(ss.str(), prog);
					ss.str("");

					ss << "Elapsed: " << monitor_->ElapsedTime() << 's';
					GUI::Text(ss.str());
					ss.str("");

					ss << "ETA: " << monitor_->RemainingTime() << 's';
					GUI::Text(ss.str());
					ss.str("");

					GUI::Divider();
					if (GUI::Button("Abort")) {
						ActionPreview();
					}
				}
				GUI::EndWindow();
			}
			else {
				GUI::BeginWindow("Settings", windowMain_);
				{
					GUI::Divider();
					if (GUI::Button("Render")) {
						ActionRender();
					}
				}
				GUI::EndWindow();
			}

			GUI::EndFrame();
		}

		void GUIViewer::ActionRender() {
			camera_->transform.UpdateMatrix();
			renderer_->Abort();
			renderer_->NewTask();
			state_ = State::Render;
			this->Refresh();
		}

		void GUIViewer::ActionPreview() {
			renderer_->Abort();
			state_ = State::Preview;
			this->Refresh();
		}

		void GUIViewer::FlipY(float *y) { *y = film_->Height() - *y - 1; }
		void GUIViewer::FlipX(float *x) { *x = film_->Width() - *x - 1; }
	}
}
