#pragma once
#include "Util/System/Application.h"
#include <memory>
#include <future>
#include <atomic>
#include "Core/Renderer.h"

namespace TX{
	namespace UI
	{
		class GUIViewer : public Application {
		public:
			enum class Direction{
				LEFT, RIGHT, UP, DOWN
			};
		public:
			GUIViewer(shared_ptr<Scene> scene, shared_ptr<Camera> camera, shared_ptr<Film> film);
			GUIViewer& ConfigRenderer(RendererConfig config);
		protected:
			void Start();
			void Config();
			bool Render();
			void OnMouseButton(MouseButton button, MouseButtonState state, Modifiers mods);
			void OnKey(KeyCode code, KeyState state, Modifiers modifiers);
			void OnResize();
			void OnExit();
		private:
			void AttemptMoveCamera(Direction dir);
			void AttemptPanCamera(Direction dir);
			void AttemptBarrelRollCamera(bool clockwise);
			void InvalidateFrame();
			void ProgressReporterJob();
			void FlipY(float *y);
			void FlipX(float *x);
		private:
			shared_ptr<Scene> scene_;
			shared_ptr<Camera> camera_;
			shared_ptr<Film> film_;
			unique_ptr<Renderer> renderer_;
			shared_ptr<IProgressMonitor> monitor_;
			std::thread progress_reporter_job_;
			bool progress_reporting;
		};
	}
}
