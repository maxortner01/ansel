#include "../headers/Keyboard.h"

#include <vector>
#include <GLFW/glfw3.h>

namespace Ansel
{
	bool Keyboard::isKeyPressed(KEY key) {
		std::vector<GLenum> k = {
			GLFW_KEY_W,
			GLFW_KEY_A,
			GLFW_KEY_S,
			GLFW_KEY_D,
			GLFW_KEY_ESCAPE,
			GLFW_KEY_LEFT_SHIFT,
			GLFW_KEY_LEFT_CONTROL,
			GLFW_KEY_SPACE,
			GLFW_KEY_UP,
			GLFW_KEY_DOWN,
			GLFW_KEY_LEFT,
			GLFW_KEY_RIGHT,
			GLFW_KEY_TAB
		};


		return glfwGetKey(Window::getWindow(), k.at(static_cast<int>(key)));
	}
}