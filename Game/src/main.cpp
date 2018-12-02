#include <Ansel.h>

#include <iostream>
#include <ctime>
#include <string>

#include "headers/GameScreen.h"

int main()
{
	//Engine::runTest();
	//std::getchar();

	// Window
	Ansel::Window window(1920, 1080);
	// Screen instance
	Game::GameScreen screen(&window);
	// Settings for engine
	Ansel::Settings settings;

	Ansel::Engine engine(&window, &screen, settings);
	engine.run();
}
