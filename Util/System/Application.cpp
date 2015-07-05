#include "System/Application.h"
#include <stdexcept>

namespace TX
{
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

		glutCreateWindow(config.title);
		if (glewInit() != GLEW_OK)
			throw std::runtime_error("glewInit failed");

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

	Application *Application::app = nullptr;
}