#pragma once
#include "fwddecl.h"

#include "Application.h"
#include <memory>
#include <future>
#include <atomic>

namespace Cruisky{
	namespace RayTracer{
		class GUIViewer : public Application {
		public:
			enum class Direction{
				LEFT, RIGHT, UP, DOWN
			};
		public:
			GUIViewer(shared_ptr<Scene> scene, shared_ptr<Camera> camera, shared_ptr<Film> film);
			void Start();
			void Config();
			GUIViewer& ConfigRenderer(RendererConfig config);
			bool Render();
			void OnMouseButton(Button button, ButtonState state, int x, int y);
			void OnKey(unsigned char c, int x, int y);
			void OnSpecialKey(KeyCode code, int x, int y);
		private:
			void AttemptMoveCamera(Direction dir);
			void AttemptPanCamera(Direction dir);
			void AttemptBarrelRollCamera(bool clockwise);
			void RenderScene();
			void AsyncRenderScene();
			void ProgressReporterJob();
			void FlipY(int *y);
			void FlipX(int *x);
		private:
			shared_ptr<Scene> scene_;
			shared_ptr<Camera> camera_;
			shared_ptr<Film> film_;
			unique_ptr<Renderer> renderer_;
			shared_ptr<IProgressMonitor> monitor_;

			std::thread progress_reporter_job_;
			std::future<void> render_task_;
			std::atomic<bool> rendering = false;
		};
	}
}