#include <Ansel.h>
#include <iostream>
#include <ctime>
#include <string>

#include "headers/MainScreen.h"
#include "headers/Terrain.h"
#include "headers/Textures.h"
#include "headers/LightTest.h"
#include "headers/FPS.h"

int main()
{
	//Engine::runTest();
	//std::getchar();

	// Window
	Ansel::Window window(1920, 1080);
	// Screen instance
	Game::LightTest screen(&window);
	// Settings for engine
	Ansel::Settings settings;

	Ansel::Engine engine(&window, &screen, settings);
	engine.run();
}
