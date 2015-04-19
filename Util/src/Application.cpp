#include "include/Application.h"
#include <stdexcept>

namespace Cruisky
{
	enum class Application::Button {
		LEFT = GLUT_LEFT_BUTTON,
		MIDDLE = GLUT_MIDDLE_BUTTON,
		RIGHT = GLUT_RIGHT_BUTTON
	};

	enum class Application::ButtonState {
		DOWN = GLUT_DOWN,
		UP = GLUT_UP
	};

	enum class Application::KeyCode {
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