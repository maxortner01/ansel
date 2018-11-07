#include <Ansel.h>
#include <iostream>

#include "headers/MainScreen.h"

int main()
{
	// Window
	Ansel::Window window(2560, 1440);
	// Screen instance
	Game::MainScreen screen(&window);
	// Settings for engine
	Ansel::Settings settings;

	Ansel::Engine engine(&window, &screen, settings);
	engine.run();
}
