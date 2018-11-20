#include <Ansel.h>
#include <iostream>

#include "headers/MainScreen.h"
#include "headers/Terrain.h"
#include "headers/Textures.h"
#include "headers/LightTest.h"

int main()
{
	// Window
	Ansel::Window window(2560, 1440);
	// Screen instance
	Game::LightTest screen(&window);
	// Settings for engine
	Ansel::Settings settings;

	Ansel::Engine engine(&window, &screen, settings);
	engine.run();
}
