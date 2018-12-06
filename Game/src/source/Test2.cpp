#include "../headers/Test2.h"

#include <iostream>

namespace Game
{
	Test2::Test2(Window* w) : Screen(w) {}

	void Test2::onCreate() {
		Loader::makeRawModel("assets/models/asteroid.obj", "cube");

		std::vector<vec4f> colors;
		RawModel* rawModel = Loader::getRawModel("cube");
		for (int i = 0; i < rawModel->getVertexCount(); i++)
			colors.push_back({ 1, 1, 1, 1 });

		rawModel->loadColors(colors);

		Loader::makeEntity("cube");
		Loader::getEntity("cube")->addComponent(Loader::makeModel(Loader::getRawModel("cube")), "cube_component");
		
		Model* model = Loader::getEntity("cube")->getComponent("cube_component")->cast<Model*>();
		model->setLocation({ 0, 0, 5, 1 });
		model->setScale({ .05f, .05f, .05f });

		//Renderer::setLighting(false);

		Light light;
		light.on = true;
		light.type = light.DIRECTIONAL;
		light.location = { 1, 0, 0 };

		Renderer::makeLight(light);
	}

	void Test2::onUpdate(float timeDelta) {
		ECS::EntityInstance a = Loader::getEntity("cube");

		Loader::getEntity("cube")->getComponent("cube_component")->cast<Model*>()->setLocation({ 0, sinf(Engine::getTime()), 5, 1 });
		Loader::getEntity("cube")->getComponent("cube_component")->cast<Model*>()->setRotation({ 0, sinf(Engine::getTime()), 5 });

		Renderer::Render(a);

		std::cout << getFPS() << std::endl;
	}
}