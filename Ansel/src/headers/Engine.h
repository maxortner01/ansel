#pragma once

#include "Window.h"
#include "Screen.h"

namespace Ansel
{

	struct ANSEL_API Settings {
		unsigned int multiSample = 0;
		bool normalizedCoords = true;
	};

	class Engine
	{
		// Window pointer
		Window* window;

		// Screen pointer
		Screen* screen;
		
		// Render the current screen
		void render();

		// Window settings
		Settings settings;


	public:
		ANSEL_API Engine(Window* w, Screen* s, Settings set = {});

		void ANSEL_API run();
	};
}