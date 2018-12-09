#include "../headers/Engine.h"
#include "../headers/systems/Directory.h"

#include <iostream>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ECS.h>
#include <AnselAlloc.h>

namespace Ansel
{
	Engine::Engine(Window* w, Screen *s, Settings set) {
		window = w; screen = s; settings = set;

		if (!settings.normalizedCoords)
			glOrtho( 0.f, window->getWidth(), window->getHeight(), 0.f, 0.f, 1.f );

		// Check if directories exist
		std::vector<std::pair<std::vector<std::string>, std::string>> dir;
		dir.push_back(std::pair<std::vector<std::string>, std::string>({"shaders"}, "ansel"));

		for (int i = 0; i < dir.size(); i++) {
			std::string base_dir = dir.at(i).second;
			if (Directory::checkFolder(base_dir.c_str()) != 0) Directory::makeFolder(base_dir.c_str());

			for (int f = 0; f < dir.at(i).first.size(); f++) {
				std::string div_name = dir.at(i).first.at(f);

				if (Directory::checkFolder(div_name.c_str()) != 0) Directory::makeFolder((base_dir + "/" + div_name).c_str());
			}

		}

	}

	void Engine::run() {
		// Initialize the Renderer
		Renderer::init(window);

		// Initialize the fps clock
		double previousTime = glfwGetTime();

		// Enable Transparency
		Renderer::frame->bind();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		Renderer::frame->unbind();

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
			Renderer::frame->clear();

			render();

			Renderer::frameShader->unbind();
			screen->renderUI((float)window->getHeight() / (float)window->getWidth());

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
		screen->onUpdate(1.f / screen->getFPS());
	}

	float Engine::getTime() {
		return glfwGetTime();
	}

	void Engine::runTest() {
		class Object : public ECS::Script
		{
			unsigned int ID = 0;

		public:
			void update() {
				ID = 25;
			}

			void* getData() {
				return &ID;
			}
		};

		ECS::EntityInstance entity = new ECS::Entity();
		entity->addComponent(new Object);

		for (int i = 0; i < entity->getComponents().size(); i++) {
			std::cout << entity->getComponents().at(i)->getType() << std::endl;

			ECS::ComponentInstance component = entity->getComponents().at(i);
			switch (component->getType()) {
			case ECS::Component::SCRIPT:
				ECS::Script* s = static_cast<ECS::Script*>(component);
				s->update();
			}

			std::cout << *(unsigned int*)(component->getData()) << std::endl;
		}

	}
}