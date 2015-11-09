#pragma once
#include "Util/System/Application.h"
#include <memory>
#include <future>
#include <atomic>
#include "Core/Renderer.h"

namespace TX {
	namespace UI {
		class GUIViewer : public InputHandledApplication {
		public:
			enum class Direction {
				LEFT, RIGHT, UP, DOWN
			};
		public:
			GUIViewer(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera, std::shared_ptr<Film> film);
			GUIViewer& ConfigRenderer(RendererConfig config);
		protected:
			void Start();
			void Config();
			bool Render();
			void OnExit();
		private:
			void OnGUI();
			void ActionRender();
			void ActionPreview();
			void FlipY(float *y);
			void FlipX(float *x);
		private:
			std::shared_ptr<Scene> scene_;
			std::shared_ptr<Camera> camera_;
			std::shared_ptr<Film> film_;
			std::unique_ptr<Renderer> renderer_;
			std::shared_ptr<IProgressMonitor> monitor_;

			// GUI
			FontMap font_;
			Rect windowMain_;
			Rect windowLog_;

			enum class State {
				Preview,
				Render,
			} state_;
		};
	}
}
