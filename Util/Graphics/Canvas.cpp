#include "Util.h"
#include "Canvas.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

namespace TX { 
	namespace UI {
		void DrawString(float x, float y, void *font, const char *text, const Color& color){
			glColor4fv((float *)&color);
			glPushMatrix();
			glRasterPos2f(x, y);
			glutBitmapString(font, (const unsigned char *)text);
			glPopMatrix();
		}

		void DrawRect(const Vector2& TL, const Vector2& BR, const Color& color, bool fill){
			glColor4fv((float *)&color);
			glPolygonMode(GL_FRONT, fill ? GL_FILL : GL_LINE);
			glBegin(GL_QUADS);
			glVertex2f(TL.x, TL.y);
			glVertex2f(BR.x, TL.y);
			glVertex2f(BR.x, BR.y);
			glVertex2f(TL.x, BR.y);
			glEnd();
		}
		void DrawTriangle(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Color& color, bool fill){
			glColor4fv((float *)&color);
			glPolygonMode(GL_FRONT, fill ? GL_FILL : GL_LINE);
			glBegin(GL_TRIANGLES);
			glVertex2f(v1.x, v1.y);
			glVertex2f(v2.x, v2.y);
			glVertex2f(v3.x, v3.y);
			glEnd();
		}
	}
}