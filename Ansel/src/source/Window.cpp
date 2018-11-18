#include "../headers/Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Ansel
{
	GLFWwindow* Window::window = NULL;

	bool Window::initializeWindow() {
		// Initialize GLFW library
		if ( !glfwInit() )
			return false;

		glfwWindowHint(GLFW_SAMPLES, 8);

		// Create windowed window with dimensions of (width, height)
		window = glfwCreateWindow(width, height, sTitle.c_str(), NULL, NULL);
		
		// Check validity of window
		if ( !window ) {
			glfwTerminate();
			return false;
		}

		// Make window current context
		glfwMakeContextCurrent(window);
		glfwSwapInterval(0);

		// Initialize GLEW library
		if (!glewInit())
			return false;

		return true;
	}

	Window::Window(unsigned int nWidth, unsigned int nHeight, std::string title) {
		width  = nWidth; height = nHeight; 
		sTitle = title;

		if (!initializeWindow())
			return;
	}

	void Window::close() { glfwSetWindowShouldClose(window, true); }

	unsigned int Window::getWidth()    const { return width;  }
	unsigned int Window::getHeight()   const { return height; }
	bool         Window::shouldClose() const { return glfwWindowShouldClose(window); }
	GLFWwindow*  Window::getWindow()		 { return window; }
	
}