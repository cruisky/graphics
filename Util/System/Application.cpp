#include "System/Application.h"
#include <stdexcept>
#include <cstdio>

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

		const char * Application::GetVersion(){
			return (const char *)glGetString(GL_VERSION);
		}

		Application *Application::app = nullptr;
	}
}