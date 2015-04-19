#pragma once

#include "Application.h"
#include "fwddecl.h"
#include <memory>
#include <future>
#include <atomic>

namespace Cruisky{
	namespace RayTracer{
		class GUIViewer : public Application {
		public:
			GUIViewer(shared_ptr<Scene> scene, shared_ptr<Camera> camera, shared_ptr<Film> film);
			void Start();
			void Config();
			bool Render();
			void OnSpecialKey(KeyCode code, int x, int y);
		private:
			void AttemptRenderScene();
			void AsyncRenderScene();
		private:
			shared_ptr<Scene> scene_;
			shared_ptr<Camera> camera_;
			shared_ptr<Film> film_;
			unique_ptr<Renderer> renderer_;

			std::future<void> task_;
			std::atomic<bool> rendering = false;
		};
	}
}