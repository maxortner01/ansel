#pragma once

#include "Window.h"
#include "Screen.h"

namespace Ansel
{
	class DECLSPECIFIER Engine
	{
		// Window pointer
		Window* window;

		// Screen pointer
		Screen* screen;
		
		// Render the current screen
		void render();

	public:
		Engine(Window* w, Screen* s);

		void run();
	};
}