#include "../headers/Keyboard.h"

namespace Ansel
{
	bool Keyboard::isKeyPressed(KEY key) {
		return glfwGetKey(Window::getWindow(), key);
	}
}