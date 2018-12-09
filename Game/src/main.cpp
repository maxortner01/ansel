#include <Ansel.h>

#include <iostream>
#include <ctime>
#include <string>
#include <vector>

#include "headers/GameScreen.h"
#include "headers/Test2.h"
#include "headers/Terrain.h"

int main()
{
	//Engine::runTest();
	//std::getchar();

	// Window
	Ansel::Window window(1920, 1080);
	// Screen instance
	Game::Test2 screen(&window);
	// Settings for engine
	Ansel::Settings settings;
	//settings.normalizedCoords = false;

	Ansel::Engine engine(&window, &screen, settings);
	engine.run();
}
