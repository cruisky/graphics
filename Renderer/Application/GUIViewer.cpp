#include "stdafx.h"

#include "txbase/image/film.h"
#include "txbase/scene/camera.h"
#include "txbase/gui/gui.h"

#include "GUIViewer.h"
#include "Core/Scene.h"
#include "Core/Intersection.h"

namespace TX {
	namespace UI {
		GUIViewer::GUIViewer(
			RendererConfig& rendererConfig,
			Scene& scene,
			Camera& camera,
			Film& film) :
			InputHandledApplication(), rendererConfig_(rendererConfig), scene_(scene), camera_(camera), film_(film) {
			config.title = "Renderer";
			config.windowSize.x = camera_.Width();
			config.windowSize.y = camera_.Height();
			config.fixsize = false;
			config.fps = 60;

			// Progress monitor
			monitor_ = std::make_unique<ProgressMonitor>();

			// Renderer
			renderer_ = std::make_unique<Renderer>(rendererConfig, scene_, camera_, film_, monitor_.get());
		}

		void GUIViewer::Start() {
			// Previewer
			previewer_ = std::make_unique<ObjViewer>(camera_, scene_);
			previewer_->lightSource.constantAttenuation = 0;
			previewer_->lightSource.linearAttenuation = 0.1f;
			previewer_->lightSource.quadraticAttenuation = 0.08f;

			InputHandledApplication::Start();
#pragma	region GUI settings
			font_.LoadDefault();
			GUI::Init(font_);
			windowMain_ = Rect(0.f, 0.f, 200.f, 180.f);
#pragma endregion
		}

		bool GUIViewer::Render() {
			if (input.windowChanged) {
				renderer_->Resize(input.windowSize.x, input.windowSize.y);
				ActionPreview();
			}

			switch (state_.mode) {
			case ViewMode::Preview:
			{
				std::shared_ptr<Light> light0 = scene_.lights[0];
				previewer_->lightSource.ambient = Color(0.15, 0.15, 0.15, 1);
				previewer_->lightSource.diffuse = light0->Intensity().Clamp();
				previewer_->lightSource.position = light0->Position();
				previewer_->lightSource.spotDirection = light0->Direction();
				previewer_->Render(state_.selected);
				break;
			}
			case ViewMode::Rendered:
				glDrawPixels(config.windowSize.x, config.windowSize.y, GL_RGBA, GL_FLOAT, (float *)film_.Pixels());
				break;
			}

			// let the GUI handle the input first
			OnGUI();

			Vec2 cursor = input.cursor;
			FlipY(&cursor.y);
			if (input(MouseButtonState::DOWN)) {
				switch (input.button) {
				case MouseButton::MIDDLE:
				{
					Color c = film_.Get(int(cursor.x), int(cursor.y));
					std::printf("(%3f, %3f), (%1.3f, %1.3f, %1.3f)\n", cursor.x, cursor.y, c.r, c.g, c.b);
					break;
				}
				case MouseButton::LEFT:
				{
					Ray r;
					Intersection intxn;
					camera_.GenerateRay(&r, cursor.x, cursor.y);
					if (scene_.Intersect(r, intxn)) {
						if (state_.selected == intxn.prim)
							state_.selected = nullptr;
						else
							state_.selected = intxn.prim;
					}
					break;
				}
				}
			}
			if (input(KeyState::DOWN) || input(KeyState::HOLD)) {
				switch (input.key) {
				case KeyCode::ESCAPE:
					Exit(); break;
				}
			}

			input.Clear();
			return true;
		}

		void GUIViewer::OnExit() {
			renderer_->Abort();
		}

		void GUIViewer::OnGUI() {
			bool rendering = renderer_->Running();
			std::stringstream ss; ss.precision(1); ss.setf(std::stringstream::fixed);
			GUI::BeginFrame(input);

			if (GUI::BeginWindow("Status", windowMain_)) {

				if (!rendering) {
					int mode = static_cast<int>(state_.mode);
					GUI::RadioButton("Rendered", mode, static_cast<int>(ViewMode::Rendered));
					GUI::RadioButton("Preview", mode, static_cast<int>(ViewMode::Preview));
					state_.mode = static_cast<ViewMode>(mode);
				}
				GUI::Divider();

				switch (state_.mode) {
				case ViewMode::Preview:
					break;
				case ViewMode::Rendered:
					if (rendering) {
						float prog = monitor_->Progress();
						ss << prog * 100.f << "% | "
							<< "ETA: " << monitor_->RemainingTime() << "s | "
							<< monitor_->ElapsedTime() << "s";
						GUI::ProgressBar(ss.str(), prog);
						ss.str("");

						GUI::Divider();
						if (GUI::Button("Abort")) {
							renderer_->Abort();
						}
					}
					break;
				}

				GUI::Divider();

				if (!rendering) {
					GUI::IntSlider("SPP", rendererConfig_.samples_per_pixel, 5, 8000);
					GUI::IntSlider("Max Depth", rendererConfig_.tracer_maxdepth, 4, 32, 4);
					if (GUI::Button("Render")) {
						ActionRender();
					}
				}
				GUI::EndWindow();
			}

			GUI::EndFrame();
		}

		void GUIViewer::ActionRender() {
			camera_.transform.UpdateMatrix();
			renderer_->Abort();
			renderer_->NewTask();
			state_.mode = ViewMode::Rendered;
			this->Refresh();
		}

		void GUIViewer::ActionPreview() {
			renderer_->Abort();
			state_.mode = ViewMode::Preview;
			this->Refresh();
		}

		void GUIViewer::FlipY(float *y) { *y = film_.Height() - *y - 1; }
		void GUIViewer::FlipX(float *x) { *x = film_.Width() - *x - 1; }
	}
}
