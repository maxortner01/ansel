#include "../headers/Test2.h"

#include <iostream>

namespace Game
{
	Mover::Mover() {
		_update = true;
	}

	void Mover::update(ECS::EntityInstance entity) {
		Model* model = entity->getComponent("cube_component")->cast<Model*>();

		model->setLocation({ 0, sinf(Engine::getTime()), 5, 1 });
		model->setRotation({ 0, sinf(Engine::getTime()), 5 });
	}

	Test2::Test2(Window* w) : Screen(w) {}

	void Test2::onCreate() {
		Loader::makeRawModel("assets/models/asteroid.obj", "asteroid");
		Loader::makeRawModel("assets/models/cube.obj", "cube");

		std::vector<vec4f> colors;
		RawModel* rawModel = Loader::getRawModel("asteroid");
		for (int i = 0; i < rawModel->getVertexCount(); i++)
			colors.push_back({ 1, 1, 1, 1 });

		rawModel->loadColors(colors);

		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 10; x++) {
				for (int z = 0; z < 10; z++) {
					ECS::EntityInstance entity = Loader::makeEntity("cube" + std::to_string(y) + "-" + std::to_string(x) + "-" + std::to_string(z));

					RawModel* rawModel = (x % 2 == 0) ? (Loader::getRawModel("asteroid")) : (Loader::getRawModel(""));
					entity->addComponent(Loader::makeModel(rawModel), "cube_component");
					Model* model = entity->getComponent("cube_component")->cast<Model*>();

					model->setLocation({ (float)y / 2.f, (float)x / 2.f, 5 + (float)z / 2.f, 1 });
					model->setScale({ .005f, .005f, .005f });
				}
			}
		}

		//Loader::getEntity("cube")->addComponent(new Mover, "controller");

		//Renderer::setLighting(false);

		Light light;
		light.on = true;
		light.type = light.DIRECTIONAL;
		light.location = { 1, 0, 0 };

		Renderer::makeLight(light);
	}

	void Test2::onUpdate(float timeDelta) {
		camera.setSpeed(.15f);
		camera.pollEvents();

		Renderer::Render(Loader::getEntities(), camera);

		std::cout << getFPS() << std::endl;
	}
}