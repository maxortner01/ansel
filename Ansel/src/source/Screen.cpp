#include "../headers/Screen.h"

namespace Ansel
{
	Screen::Draw::Draw() {}
	Screen::Screen() {}

	void Screen::Draw::line(Ansel::vec2f startpoint, Ansel::vec2f endpoint) {
		glBegin(GL_LINES);

			glVertex2f(startpoint.x, startpoint.y);
			glVertex2f(endpoint.x, endpoint.y);

		glEnd();
	}

	void Screen::onCreate( )  {}
	void Screen::onDestroy( ) {}
}