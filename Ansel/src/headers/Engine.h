#pragma once

#include "Renderer.h"
#include "Window.h"
#include "Screen.h"

namespace Ansel
{
	/*! 
	 * Struct for the user to set OpenGL context settings.
	 */
	struct ANSEL_API Settings {
		unsigned int multiSample = 0;
		bool normalizedCoords = true;
	};

	/*!
	 * All engine operations fall within this class. Runs all code.
	 */
	class Engine
	{
		Window* window; /*!< Pointer to Window instance */

		Screen* screen; /*!< Pointer to user-made screen instance */
		
		void render();  /*!< Runs the screen's render function */

		Settings settings; /*!< Instance of window's settings */

	public:
		ANSEL_API Engine(Window* w, Screen* s, Settings set = {});

		void ANSEL_API run();

		static ANSEL_API float getTime();
	};
}