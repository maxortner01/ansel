#include <Ansel.h>
#include <iostream>

#include "headers/MainScreen.h"

int main()
{
	// Window
	Ansel::Window window(500, 500);
	// Screen instance
	Game::MainScreen screen;
	// Settings for engine
	Ansel::Settings settings;
	settings.normalizedCoords = true;

	Ansel::Engine engine(&window, &screen, settings);
	engine.run();
}
