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
		Renderer::init(window);

		// Initialize the fps clock
		double previousTime = glfwGetTime();


		// Enable Transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		// Run the current screen's create function and set the window dimensions
		// within the screen
		screen->ScreenSize = { window->getWidth(), window->getHeight() };
		screen->onCreate();

		while ( !window->shouldClose() ) {
			// First calculate FPS

			double currentTime = glfwGetTime();
			screen->fps = 1.f / (float)(currentTime - previousTime);
			previousTime = currentTime;

			// Next check next screen

			// Check if the current screen's next screen is not NULL
			// If not, that means its time to move on to that screen
			Screen* nextScreen = screen->getNextScreen();
			if ( nextScreen != NULL )
				screen = nextScreen;

			// Poll window events
			glfwPollEvents();

			// Then start rendering

			// Clear the window
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Bind the framebuffer and clear it
			Renderer::frame->bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);
			Renderer::frame->unbind();

			// Render the screen
			render();

			// Finally, when everything is rendered, render the frame to the window
			Renderer::renderFrame();

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

	float Engine::getTime() {
		return glfwGetTime();
	}

}