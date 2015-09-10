#pragma once

#include "Util.h"
#include "Math/Vector.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

namespace TX{ namespace UI{
	enum class MouseButton {
		NONE = -1,
		LEFT = GLUT_LEFT_BUTTON,
		MIDDLE = GLUT_MIDDLE_BUTTON,
		RIGHT = GLUT_RIGHT_BUTTON,
		WHEEL_UP,
		WHEEL_DOWN
	};
	enum class MouseButtonState{
		DOWN = GLUT_DOWN,
		UP = GLUT_UP
	};
	enum class KeyCode{
		NONE = -1,
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
	enum class Modifier {
		SHIFT = 0,
		ALT = 1,
		CTRL = 2
	};
	struct Input {
		Vector2 screen;
		Vector2 mouse;
		MouseButton button;
		MouseButtonState buttonState;
		int scroll;
		unsigned char key;
		KeyCode specialKey[3]; int specialKeyCount;
		bool modifier[3];

		inline void Clear(){
			button = MouseButton::NONE;
			scroll = 0;
			ClearKey();
			specialKeyCount = 0;
			modifier[0] = modifier[1] = modifier[2] = false;
		}
		inline void SetScreen(int w, int h){ screen.x = (float)w; screen.y = (float)h; }
		inline void SetMouse(float x, float y){ mouse.x = x, mouse.y = y; }
		inline void SetButton(MouseButton b, MouseButtonState s){ button = b; buttonState = s; }
		inline bool HasKey(){ return key != 0; }
		inline void AddKey(unsigned char c){ if (!key) key = c; }
		inline void ClearKey(){ key = 0; }
		inline void AddSpecialKey(KeyCode code){ if (specialKeyCount < 3) specialKey[specialKeyCount++] = code; }
		inline void SetModifier(Modifier code){ modifier[static_cast<int>(code)] = true; }
	};
}}