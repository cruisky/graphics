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
			Application() : argc(1), argv(nullptr){ config.title = "Application"; }
			Application(int argc, char **argv) : argc(argc), argv(argv){}
			virtual ~Application(){}
			void Run();

			// Configures the window property
			virtual void Config(){}
			// Called before main loop
			virtual void Start(){}
			// Rendering loop, returns true if need update
			virtual bool Render(){ return false; }

			virtual void OnResize(){}
			virtual void OnKey(unsigned char key, int mousex, int mousey){}						// ASCII keys
			virtual void OnSpecialKey(KeyCode key, int mousex, int mousey){}					// special keys
			virtual void OnMouseMove(int x, int y){}											// when mouse moves with/without any pressed button
			virtual void OnMouseButton(MouseButton button, MouseButtonState state, int x, int y){}		// when a mouse button is pressed / release
			virtual void OnExit(){}

		protected:
			float ElapsedTime();
			void Exit();
			// Wrappers
		private:
			static const char * GetVersion();
			static void GLUTRender();
			static void GLUTKey(unsigned char key, int x, int y);
			static void GLUTSpecial(int key, int x, int y);
			static void GLUTMouseDrag(int x, int y);
			static void GLUTMouseMove(int x, int y);
			static void GLUTMouseButton(int b, int s, int x, int y);
			static void GLUTResize(int w, int h);
		protected:
			AppConfig config;
		private:
			int argc;
			char **argv;
			static Application *app;
		};
	}
}