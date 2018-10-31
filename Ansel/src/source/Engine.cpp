#include "../headers/Engine.h"

namespace Ansel
{
	Engine::Engine(Window* w, Screen *s) {
		window = w; screen = s;
	}

	void Engine::run() {

		while ( !window->shouldClose() ) {
			glClear(GL_COLOR_BUFFER_BIT);

			render();

			glfwSwapBuffers(window->getWindow());

			glfwPollEvents();
		}

	}

	void Engine::render() {
		screen->onUpdate();
	}

}