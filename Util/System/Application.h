#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <cstdio>
#include <string>
#include "Math/Vector.h"
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
			float ElapsedTime(){ return glutGet(GLUT_ELAPSED_TIME) / 1000.f; }
			void Exit(){ OnExit(); glutLeaveMainLoop(); }
			// Wrappers
		private:
			static const char * GetVersion();
			static inline void GLUTRender() {
				if (app->Render()){
					glutPostRedisplay();
				}
				glutSwapBuffers();
			}
			static inline void GLUTKey(unsigned char key, int x, int y)		{ app->OnKey(key, x, y); }
			static inline void GLUTSpecial(int key, int x, int y)			{ app->OnSpecialKey(KeyCode(key), x, y); }
			static inline void GLUTMouseDrag(int x, int y)					{ app->OnMouseMove(x, y); }
			static inline void GLUTMouseMove(int x, int y)					{ app->OnMouseMove(x, y); }
			static inline void GLUTMouseButton(int b, int s, int x, int y)	{ app->OnMouseButton(MouseButton(b), MouseButtonState(s), x, y); }
			static inline void GLUTResize(int w, int h)	{
				if (w != app->config.width || h != app->config.height){
					if (!app->config.fixsize){
						app->config.width = w;
						app->config.height = h;
						app->OnResize();
					}
					glutReshapeWindow(app->config.width, app->config.height);
				}
			}
		protected:
			AppConfig config;
		private:
			int argc;
			char **argv;
			static Application *app;
		};
	}
}