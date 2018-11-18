#pragma once

#include "Def.h"

class GLFWwindow;

namespace Ansel
{
	class ANSEL_API Window
	{
		// Width and Height of the window
		unsigned int width, height;

		// Title of the window
		std::string sTitle;

		// Create and initialize glfw/glew context
		bool initializeWindow();

		// GLFW window context
		static GLFWwindow* window;

	public:
		// Create window instance and initialize
		Window(unsigned int nWidth, unsigned int nHeight, std::string title = "Engine");

		// Close window
		static void close( );

		// Get window width
		unsigned int getWidth( )  const;
		// Get window height
		unsigned int getHeight( ) const;
		// Get the state of the window (opened/closed)
		bool shouldClose( ) const;
		// Get glfw window instance
		static GLFWwindow* getWindow( );
	};
}