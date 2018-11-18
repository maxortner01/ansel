#include <Ansel.h>
#include <iostream>

#include "headers/MainScreen.h"
#include "headers/Terrain.h"
#include "headers/Textures.h"

int main()
{
	// Window
	Ansel::Window window(1920, 1080);
	// Screen instance
	Game::Terrain screen(&window);
	// Settings for engine
	Ansel::Settings settings;

	Ansel::Engine engine(&window, &screen, settings);
	engine.run();
}
