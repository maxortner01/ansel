#include "../headers/GameScreen.h"

#include <iostream>

using namespace Ansel;

namespace Game
{
	GameScreen::GameScreen(Window *w) : Screen(w) {}

	void GameScreen::onCreate() {
		Loader::makeRawModel("assets/models/cube.obj", "cube");
		
		std::vector<vec4f> colors;
		RawModel* rawModel = Loader::getRawModel("cube");
		for (int i = 0; i < rawModel->getVertexCount(); i++)
			colors.push_back({ 1, 1, 1, 1 });

		rawModel->loadColors(colors);

		Loader::makeEntity("player");
		Loader::getEntity("player")->addComponent(Loader::makeModel(Loader::getRawModel("cube")), "model");
		Loader::getEntity("player")->addComponent(new Object, "health");

		Loader::getEntity("player")->getComponent("model")->cast<Model*>()->setLocation({ 0, 0, 5, 1 });

		Renderer::setLighting(false);
	}

	void GameScreen::onUpdate(float timeDelta) {
		ECS::EntityInstance a = Loader::getEntity("player");

		Object* health = a->getComponent("health")->cast<Object*>();
		std::cout << health->getValue() << std::endl;

		Renderer::Render(a);

		//std::cout << 1.f / timeDelta << std::endl;
	}
}