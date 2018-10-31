#include <Ansel.h>
#include <iostream>

#include "headers/MainScreen.h"

int main()
{
	Ansel::Window window(500, 500);
	Game::MainScreen screen;

	Ansel::Engine engine(&window, &screen);
	engine.run();
}
