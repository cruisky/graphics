#pragma once
#include <memory>
#include <future>
#include <atomic>
#include "txbase/opengl/application.h"

#include "Core/Renderer.h"
#include "Application/ObjViewer.h"

namespace TX {
	namespace UI {
		class GUIViewer : public InputHandledApplication {
		public:
			enum class Direction {
				LEFT, RIGHT, UP, DOWN
			};
		public:
			GUIViewer(
				RendererConfig& config,
				Scene& scene,
				Camera& camera,
				Film& film);
		protected:
			void Start();
			bool Render();
			void OnExit();
		private:
			void OnGUI();
			void ActionRender();
			void ActionPreview();
			void FlipY(float *y);
			void FlipX(float *x);
		private:
			Scene& scene_;
			Camera& camera_;
			Film& film_;
			std::unique_ptr<Renderer> renderer_;
			RendererConfig& rendererConfig_;
			std::unique_ptr<ObjViewer> previewer_;
			std::unique_ptr<IProgressMonitor> monitor_;

			// GUI
			FontMap font_;
			Rect windowMain_;
			Rect windowLog_;

			enum class ViewMode {
				Preview,
				Rendered,
			};
			struct {
				ViewMode mode;
				const Primitive *selected;
			} state_;


		};
	}
}
