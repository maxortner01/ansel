#pragma once

#include "Window.h"
#include "Screen.h"

namespace Ansel
{
	struct ANSEL_API Settings {
		unsigned int multiSample = 0;
		bool normalizedCoords = true;
	};

	class ANSEL_API Engine
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
		Engine(Window* w, Screen* s, Settings set = {});

		void run();
	};
}