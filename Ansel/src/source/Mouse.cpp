#include "../headers/Mouse.h"

#include <GLFW/glfw3.h>

namespace Ansel
{

	vec2d Mouse::getPostion(bool off_screen) { 
		vec2d location;
		vec2i dim;

		glfwGetWindowSize(Window::getWindow(), &dim.x, &dim.y);
		glfwGetCursorPos(Window::getWindow(), &location.x, &location.y);

		if (!off_screen) {
			/**/ if (location.x < 0)
				location.x = 0;
			else if (location.x > dim.x)
				location.x = dim.x;

			/**/ if (location.y < 0)
				location.y = 0;
			else if (location.y > dim.y)
				location.y = dim.y;
		}

		return location;
	}

	void Mouse::setPosition(vec2f position) {
		glfwSetCursorPos(Window::getWindow(), position.x, position.y);
	}

	void Mouse::hideCursor() {
		glfwSetInputMode(Window::getWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
}