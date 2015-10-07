#include "UtilStdAfx.h"
#include "System/Application.h"

namespace TX
{
	namespace UI{
		void Application::Run(){
			app = this;
			Config();

			if (!argv){
				// create fake argv
				argc = 1;
				argv = new char*[2];
				argv[0] = "fakeprogram";
				argv[1] = NULL;
				glutInit(&argc, argv);
				delete[] argv;
			}
			else {
				glutInit(&argc, argv);
			}

			glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
			glutInitWindowSize(config.width, config.height);

			glutCreateWindow(config.title.data());
			if (glewInit() != GLEW_OK)
				throw std::runtime_error("glewInit failed");

			printf("OpenGL Version: \t%s\n", GetVersion());

			if (config.fullscreen)
				glutFullScreen();

			glutDisplayFunc(GLUTRender);			// main loop
			glutReshapeFunc(GLUTResize);			// resize
			glutKeyboardFunc(GLUTKey);				// ascii keys
			glutSpecialFunc(GLUTSpecial);			// non-ascii keys
			glutMotionFunc(GLUTMouseDrag);			// mouse drag
			glutPassiveMotionFunc(GLUTMouseMove);	// mouse move
			glutMouseFunc(GLUTMouseButton);			// mouse button

			Start();
			glutMainLoop();
		}
		float Application::ElapsedTime(){ return glutGet(GLUT_ELAPSED_TIME) / 1000.f; }
		void Application::Exit(){ OnExit(); glutLeaveMainLoop(); }
		void Application::GLUTRender(){
			 if (app->Render()){
				 glutPostRedisplay();
			 }
			 glutSwapBuffers();
		 }
		void Application::GLUTKey(unsigned char key, int x, int y)		{ app->OnKey(key, x, y); }
		void Application::GLUTSpecial(int key, int x, int y)			{ app->OnSpecialKey(KeyCode(key), x, y); }
		void Application::GLUTMouseDrag(int x, int y)					{ app->OnMouseMove(x, y); }
		void Application::GLUTMouseMove(int x, int y)					{ app->OnMouseMove(x, y); }
		void Application::GLUTMouseButton(int b, int s, int x, int y)	{ app->OnMouseButton(MouseButton(b), MouseButtonState(s), x, y); }
		void Application::GLUTResize(int w, int h)	{
			if (w != app->config.width || h != app->config.height){
				if (!app->config.fixsize){
					app->config.width = w;
					app->config.height = h;
					app->OnResize();
				}
				glutReshapeWindow(app->config.width, app->config.height);
			}
		}
		const char * Application::GetVersion(){
			return (const char *)glGetString(GL_VERSION);
		}

		Application *Application::app = nullptr;
	}
}