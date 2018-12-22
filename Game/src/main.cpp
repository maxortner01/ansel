#include <Ansel.h>

#include <iostream>
#include <ctime>
#include <string>
#include <vector>

//#include "headers/GameScreen.h"
//#include "headers/Test2.h"
//#include "headers/Terrain.h"

#include "demo1/demo1.h"

int main()
{
	//Engine::runTest();
	//std::getchar();

	// Window
	Ansel::Window window(1920, 1080);
	// Screen instance
	Demo1::DemoScreen screen(&window);
	// Settings for engine
	Ansel::Settings settings;
	//settings.normalizedCoords = false;

	Ansel::Engine engine(&window, &screen, settings);
	engine.run();
}
