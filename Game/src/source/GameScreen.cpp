#include "../headers/GameScreen.h"

#include <iostream>

using namespace Ansel;

namespace Game
{
	GameScreen::GameScreen(Window *w) : Screen(w) {}

	void GameScreen::onCreate() {
		Loader::makeEntity("cool_beans");
	}

	void GameScreen::onUpdate(float timeDelta) {
		ECS::EntityInstance a = Loader::getEntity("cool_beans");
		

		std::cout << 1.f / timeDelta << std::endl;
	}
}