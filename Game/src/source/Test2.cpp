#include "../headers/Test2.h"

#include <iostream>

namespace Game
{
	Mover::Mover() {
		_update = true;
	}

	void Mover::update(ECS::EntityInstance entity) {
		Model* model = entity->getComponent("cube_component")->cast<Model*>();

		//model->setLocation({ 0, sinf(Engine::getTime()), 5, 1 });
		//model->setRotation({ 0, sinf(Engine::getTime() + model->getLocation().z), 5 });
	}

	Test2::Test2(Window* w) : Screen(w) {}

	void Test2::onCreate() {
		Loader::makeRawModel("assets/models/asteroid.obj", "asteroid");
		Loader::makeRawModel("assets/models/cube.obj", "cube", true);

		std::vector<std::string> names = { "asteroid", "cube" };
		
		for (int i = 0; i < names.size(); i++) {
			std::vector<vec4f> colors;
			RawModel* rawModel = Loader::getRawModel(names.at(i).c_str());
			for (int i = 0; i < rawModel->getVertexCount(); i++)
				colors.push_back({ 1, 1, 1, 1 });

			rawModel->loadColors(colors);
		}

		float space = 5.f;
		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 10; x++) {
				for (int z = 0; z < 10; z++) {
					//if (x % 2 == 0) continue;
					ECS::EntityInstance entity = Loader::makeEntity("cube" + std::to_string(y) + "-" + std::to_string(x) + "-" + std::to_string(z));

					RawModel* rawModel = (x % 2 == 0) ? (Loader::getRawModel("asteroid")) : (Loader::getRawModel("cube"));

					entity->addComponent(new Mover, "mover");
					entity->addComponent(Loader::makeModel(rawModel), "cube_component");
					Model* model = entity->getComponent("cube_component")->cast<Model*>();

					model->setLocation({ (float)x / 2.f * space, (float)y / 2.f * space, 5 + ((float)z / 2.f * space), 1 });
					if (x % 2 == 0) model->setScale({ .005f * space, .005f * space, .005f * space});
					else model->setScale({ .07f * space, .07f * space, .07f * space});
				}
			}
		}

		//Loader::getEntity("cube")->addComponent(new Mover, "controller");

		//Renderer::setLighting(false);

		Light light;
		light.on = true;
		light.type = light.DIRECTIONAL;
		light.location = { 0, 1, -1 };

		Renderer::makeLight(light);
	}

	void Test2::onUpdate(float timeDelta) {
		camera.setSpeed(.25f);
		camera.pollEvents();

		Renderer::Render(Loader::getEntities(), camera);

		std::cout << getFPS() << std::endl;
	}
}