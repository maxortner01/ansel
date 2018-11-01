#include "../headers/Engine.h"

namespace Ansel
{
	Engine::Engine(Window* w, Screen *s, Settings set) {
		window = w; screen = s; settings = set;

		if (!settings.normalizedCoords)
			glOrtho( 0.f, window->getWidth(), window->getHeight(), 0.f, 0.f, 1.f );
	}

	void Engine::run() {

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glClearColor(0.0, 0.0, 0.0, 0.0);
		while ( !window->shouldClose() ) {
			// Check if the current screen's next screen is not NULL
			// If not, that means its time to move on to that screen
			Screen* nextScreen = screen->getNextScreen();
			if ( nextScreen != NULL )
				screen = nextScreen;

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