#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <cstdio>

namespace TX{
	class Application {
	public:
		struct AppConfig {
			char title[128];
			int width = 800;
			int height = 600;
			bool fullscreen = false;
			bool fixsize = false;
		};
		enum class Button {
			LEFT = GLUT_LEFT_BUTTON,
			MIDDLE = GLUT_MIDDLE_BUTTON,
			RIGHT = GLUT_RIGHT_BUTTON
		};
		enum class ButtonState{
			DOWN = GLUT_DOWN,
			UP = GLUT_UP
		};
		enum class KeyCode{
			F1 = GLUT_KEY_F1,
			F2 = GLUT_KEY_F2,
			F3 = GLUT_KEY_F3,
			F4 = GLUT_KEY_F4,
			F5 = GLUT_KEY_F5,
			F6 = GLUT_KEY_F6,
			F7 = GLUT_KEY_F7,
			F8 = GLUT_KEY_F8,
			F9 = GLUT_KEY_F9,
			F10 = GLUT_KEY_F10,
			F11 = GLUT_KEY_F11,
			F12 = GLUT_KEY_F12,
			LEFT = GLUT_KEY_LEFT,
			UP = GLUT_KEY_UP,
			RIGHT = GLUT_KEY_RIGHT,
			DOWN = GLUT_KEY_DOWN,
			PAGE_UP = GLUT_KEY_PAGE_UP,
			PAGE_DOWN = GLUT_KEY_PAGE_DOWN,
			HOME = GLUT_KEY_HOME,
			END = GLUT_KEY_END,
			INSERT = GLUT_KEY_INSERT,
			NUMLOCK = GLUT_KEY_NUM_LOCK,
			BEGIN = GLUT_KEY_BEGIN,
			DEL = GLUT_KEY_DELETE,
			SHIFT_L = GLUT_KEY_SHIFT_L,
			SHIFT_R = GLUT_KEY_SHIFT_L,
			CTRL_L = GLUT_KEY_CTRL_L,
			CTRL_R = GLUT_KEY_CTRL_R,
			ALT_L = GLUT_KEY_ALT_L,
			ALT_R = GLUT_KEY_ALT_R
		};

	public:
		Application() : argc(1), argv(nullptr){}
		Application(int argc, char **argv) : argc(argc), argv(argv){}
		virtual ~Application(){}
		void Run();

		// Configures the window property
		virtual void Config(){ strcpy_s(config.title, "Application"); }
		// Called before main loop
		virtual void Start(){}
		// Rendering loop, returns true if need update
		virtual bool Render(){ return false; }
		
		virtual void OnResize(){}
		virtual void OnKey(unsigned char key, int mousex, int mousey){}						// ASCII keys
		virtual void OnSpecialKey(KeyCode key, int mousex, int mousey){}					// special keys
		virtual void OnMouseMove(int x, int y){}											// when mouse moves with/without any pressed button
		virtual void OnMouseButton(Button button, ButtonState state, int x, int y){}		// when a mouse button is pressed / release
		virtual void OnExit(){}

	protected:
		float ElapsedTime(){ return glutGet(GLUT_ELAPSED_TIME) / 1000.f; }
		void Exit(){ OnExit(); glutLeaveMainLoop(); }
	// Wrappers
	private:
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
		static inline void GLUTMouseButton(int b, int s, int x, int y)	{ app->OnMouseButton(Button(b), ButtonState(s), x, y); }
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