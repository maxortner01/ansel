#include "../headers/GameScreen.h"

#include <iostream>

using namespace Ansel;

namespace Game
{
	GameScreen::GameScreen(Window *w) : Screen(w) {}

	void GameScreen::onCreate() {
		Loader::makeRawModel("assets/models/cube.obj", "cube");

		Loader::makeEntity("player");
		Loader::getEntity("player")->addComponent(Loader::makeModel(Loader::getRawModel("cube")), "model");
		Loader::getEntity("player")->addComponent(new Object, "health");
	}

	void GameScreen::onUpdate(float timeDelta) {
		ECS::EntityInstance a = Loader::getEntity("player");

		Object* health = static_cast<Object*>(a->getComponent("health"));
		std::cout << health->getValue() << std::endl;

		Renderer::Render(a);

		//std::cout << 1.f / timeDelta << std::endl;
	}
}