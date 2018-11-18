#include "../headers/Engine.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Ansel
{
	Engine::Engine(Window* w, Screen *s, Settings set) {
		window = w; screen = s; settings = set;

		if (!settings.normalizedCoords)
			glOrtho( 0.f, window->getWidth(), window->getHeight(), 0.f, 0.f, 1.f );
	}

	void Engine::run() {
		// Initialize the Renderer
		Renderer::init({ window->getWidth(), window->getHeight() });

		// Initialize the fps clock
		double previousTime = glfwGetTime();

		// Enable Transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		//glClearColor(0.0, 0.0, 0.75, 1.0);

		// Run the current screen's create function
		screen->ScreenSize = { window->getWidth(), window->getHeight() };
		screen->onCreate();

		while ( !window->shouldClose() ) {
			// FPS Calculations
			double currentTime = glfwGetTime();
			screen->fps = 1.f / (float)(currentTime - previousTime);
			previousTime = currentTime;

			// Check if the current screen's next screen is not NULL
			// If not, that means its time to move on to that screen
			Screen* nextScreen = screen->getNextScreen();
			if ( nextScreen != NULL )
				screen = nextScreen;

			// Clear the color and depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Poll window events
			glfwPollEvents();

			// Render the screen
			render();

			// Swap the buffers
			glfwSwapBuffers(window->getWindow());

			glFlush();
			Renderer::uFrame++;
		}

		// Run the current screen's destroy function
		screen->onDestroy();

		// Destroy all VAO and VBO objects
		Loader::destroy();

	}

	void Engine::render() {
		screen->onUpdate();
	}

}