#pragma once

#include "Util.h"
#include "Input.h"

namespace TX{
	namespace UI {
		class Application {
		public:
			struct AppConfig {
				std::string title;
				int width = 800;
				int height = 600;
				bool fullscreen = false;
				bool fixsize = false;
			};
		public:
			Application();
			~Application();
			void Run();

		protected:
			/// <summary>
			/// Configures the window property.
			/// </summary>
			virtual void Config(){}
			/// <summary>
			/// Called before main loop.
			/// </summary>
			virtual void Start(){}
			/// <summary>
			/// Rendering loop, returns true if need update.
			/// </summary>
			virtual bool Render(){ return false; }

			virtual void OnKey(KeyCode code, KeyState state, Modifiers modifiers){}
			virtual void OnText(uint code, Modifiers modifiers){}
			virtual void OnMouseMove(float x, float y){}
			virtual void OnMouseScroll(float vx, float vy){}
			virtual void OnMouseButton(MouseButton button, MouseButtonState state, Modifiers mods){}
			virtual void OnResize(){}
			virtual void OnWindowResize(int w, int h){}
			virtual void OnWindowFocusChanged(bool focused){}
			virtual void OnWindowPos(int x, int y){}
			virtual void OnWindowMinimize(){}
			virtual void OnExit(){}

		protected:
			static const char * GetVersion();
			MouseButtonState	Get(MouseButton button);
			bool				Get(KeyCode code);
			float				GetTime();
			void				GetCursorPos(float *x, float *y);
			void				Refresh();
			bool				IsWindowVisible();
			void				Exit();
		private:
			static Application * This(GLFWwindow *window);
			static void GLFWKey(GLFWwindow *window, int key, int scancode, int action, int mods);
			static void GLFWCharMods(GLFWwindow *window, uint codepoint, int mods);
			static void GLFWCursorPos(GLFWwindow *window, double x, double y);
			static void GLFWMouseButton(GLFWwindow *window, int b, int s, int mods);
			static void GLFWMouseScroll(GLFWwindow *window, double xoff, double yoff);
			static void GLFWFramebufferSize(GLFWwindow *window, int w, int h);
			static void GLFWWindowSize(GLFWwindow *window, int w, int h);
			static void GLFWWindowFocus(GLFWwindow *window, int focused);
			static void GLFWWindowPos(GLFWwindow *window, int x, int y);
			static void GLFWWindowIconify(GLFWwindow *window, int iconified);
			static void GLFWError(int error, const char* desc);
		protected:
			AppConfig config;
		private:
			GLFWwindow *window;
		};
	}
}
