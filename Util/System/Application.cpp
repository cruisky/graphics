#include "UtilStdAfx.h"
#include "System/Application.h"
#include <thread>
#include <chrono>

namespace TX
{
	namespace UI{
		Application::Application() { config.title = "Application"; }
		void Application::Run(){
			Config();

			glfwSetErrorCallback(GLFWError);
			if (!glfwInit())
				throw std::runtime_error("glfwInit failed");

			window = glfwCreateWindow(
				config.windowSize.x,
				config.windowSize.y,
				config.title.c_str(),
				config.fullscreen ? glfwGetPrimaryMonitor() : NULL,
				NULL);
			if (!window){
				glfwTerminate();
				throw std::runtime_error("glfwCreateWindow failed");
			}
			glfwSetWindowUserPointer(window, this);
			glfwMakeContextCurrent(window);
			glfwSwapInterval(1);

			glfwSetKeyCallback(window, GLFWKey);
			glfwSetCharModsCallback(window, GLFWCharMods);
			glfwSetCursorPosCallback(window, GLFWCursorPos);
			glfwSetMouseButtonCallback(window, GLFWMouseButton);
			glfwSetScrollCallback(window, GLFWMouseScroll);
			glfwSetFramebufferSizeCallback(window, GLFWFramebufferSize);
			glfwSetWindowSizeCallback(window, GLFWWindowSize);
			glfwSetWindowFocusCallback(window, GLFWWindowFocus);
			glfwSetWindowPosCallback(window, GLFWWindowPos);
			glfwSetWindowIconifyCallback(window, GLFWWindowIconify);

			if (glewInit() != GLEW_OK)
			{
				throw std::runtime_error("glewInit failed");
			}
			std::printf("OpenGL Version: \t%s\n", GetVersion());

			Start();

			// Rendering loop
			bool realtime = true;
			while (!glfwWindowShouldClose(window)){
				FrameStart();
				realtime = Render();
				glfwSwapBuffers(window);
				if (realtime)
					glfwPollEvents();
				else
					glfwWaitEvents();
				FrameEnd();
			}

			OnExit();
			glfwDestroyWindow(window);
			glfwTerminate();
		}
		Application::~Application(){}

		MouseButtonState	Application::Get(MouseButton button) { return MouseButtonState(glfwGetMouseButton(window, static_cast<int>(button))); }
		bool				Application::Get(KeyCode code){ return glfwGetKey(window, static_cast<int>(code)) == GLFW_PRESS; }
		const char *		Application::GetVersion(){ return (const char *)glGetString(GL_VERSION); }
		float				Application::GetTime() { return (float)glfwGetTime(); }
		float				Application::GetDeltaTime() { return deltaTime; };
		float				Application::GetFrameRate() { return fps; }
		void				Application::GetCursorPos(float *x, float *y){ double dx, dy; glfwGetCursorPos(window, &dx, &dy); *x = float(dx); *y = float(dy); }
		void				Application::Refresh(){ glfwPostEmptyEvent(); }
		bool				Application::IsWindowVisible(){ return glfwGetWindowAttrib(window, GLFW_VISIBLE); }
		void				Application::Exit(){ glfwSetWindowShouldClose(window, GL_TRUE); }

		Application * Application::This(GLFWwindow *window){ return (Application *)(glfwGetWindowUserPointer(window)); }
		void Application::GLFWKey(GLFWwindow *window, int key, int scancode, int action, int mods){
			This(window)->OnKey(KeyCode(key), KeyState(action), mods);
		}
		void Application::GLFWCharMods(GLFWwindow *window, uint codepoint, int mods){
			This(window)->OnText(codepoint, mods);
		}
		void Application::GLFWCursorPos(GLFWwindow *window, double x, double y){
			This(window)->OnMouseMove(float(x), float(y));
		}
		void Application::GLFWMouseButton(GLFWwindow *window, int button, int state, int mods){
			This(window)->OnMouseButton(MouseButton(button), MouseButtonState(state), mods);
		}
		void Application::GLFWMouseScroll(GLFWwindow *window, double xoff, double yoff) { This(window)->OnMouseScroll(float(xoff), float(yoff)); }
		void Application::GLFWFramebufferSize(GLFWwindow *window, int w, int h)	{
			auto *app = This(window);
			Vec2i newSize(w, h);
			if (app->config.windowSize != newSize){
				if (!app->config.fixsize){
					app->config.windowSize = newSize;
					app->OnResize();
					glViewport(0, 0, w, h);
				}
			}
		}
		void Application::GLFWWindowSize(GLFWwindow *window, int w, int h)	{ This(window)->OnWindowResize(w, h); }
		void Application::GLFWWindowFocus(GLFWwindow *window, int focused) { This(window)->OnWindowFocusChanged(focused); }
		void Application::GLFWWindowPos(GLFWwindow *window, int x, int y) { This(window)->OnWindowPos(x, y); }
		void Application::GLFWWindowIconify(GLFWwindow *window, int iconified) { This(window)->OnWindowMinimize(); }
		void Application::GLFWError(int error, const char* desc){
			std::cerr << "GLFWError " << error << ": " << desc << std::endl;
		}

		void Application::FrameStart() {
			float now = GetTime();
			deltaTime = now - frameStart;
			fps = Math::Lerp(0.95f, 1.f / deltaTime, fps);
			frameStart = now;
			//glfwSetWindowTitle(window, (config.title + '[' + std::to_string(int(GetFrameRate())) + ']').c_str());
		}

		void Application::FrameEnd() {
			float now = GetTime();
			frameEnd = now;

			// limit fps
			float deadLine = frameStart + 1.f / config.fps;
			while (deadLine > now) {
				float waitTime = (deadLine - now) * 0.6f;
				if (waitTime > 0.005f) {
					std::this_thread::sleep_for(std::chrono::milliseconds(int(waitTime * 1000)));
				}
				now = GetTime();
			}
		}
	}
}
