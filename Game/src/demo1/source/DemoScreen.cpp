#include "../headers/DemoScreen.h"

#include "../headers/Chunk.h"

#include <thread>
#include <iostream>

using namespace Ansel;

namespace Demo1 {
	void print(float fps) {
		std::cout << fps << std::endl;
	}

	DemoScreen::DemoScreen(Window* w) : Screen(w) {}

	void DemoScreen::onCreate() {
		Loader::makeEntity(new Skybox(&camera), "skybox");

		std::vector<std::thread*> threads;
		std::vector<ModelContainer*> containers;

		for (int z = -4; z <= 4; z++) {
			for (int x = -4; x <= 4; x++) {
				vec2i loc = { x, z };

				containers.push_back(new ModelContainer);
				threads.push_back(new std::thread(Chunk::genChunk, containers.at(containers.size() - 1), loc));
			}
		}

		Material* material = new Material;
		material->loadTexture(Material::ALBEDO, new Texture("assets/textures/good_sand.png"));
		material->loadTexture(Material::NORMAL, new Texture("assets/textures/good_sand_normal.png"));

		for (int i = 0; i < threads.size(); i++) {
			threads.at(i)->join();

			RawModel* _rawModel = Loader::makeRawModel(containers.at(i)->vertices, containers.at(i)->indices, containers.at(i)->normals, std::to_string(i).c_str());
			_rawModel->loadColors(containers.at(i)->colors);
			_rawModel->loadTextureCoordinates(containers.at(i)->tex);

			_rawModel->loadMaterial(material);

			ECS::EntityInstance chunk = new Chunk(_rawModel);
			Loader::makeEntity(chunk);
		}

		for (int i = 0; i < threads.size(); i++) {
			delete threads.at(i);
			delete containers.at(i);
		}

		Light light;
		light.on = true;
		light.type = light.DIRECTIONAL;
		light.location = { 0, 1, 1 };
		light.color = { .65f, .65f, .65f };

		Renderer::makeLight(light);
		//Renderer::setLighting(false);
	}

	void DemoScreen::onUpdate(float timeDelta) {
		std::thread printThread = std::thread(print, getFPS());

		Light* light = Renderer::getLight(0);
		light->location = {cosf(Engine::getTime() / 20.f), sinf(Engine::getTime() / 20.f), 0};

		if (Keyboard::isKeyPressed(KEY::ESCAPE))
			window->close();

		float rotation_speed = .1f * 3.14159f / 180.f;
		vec2d newMouse = Mouse::getPostion();
		Mouse::setPosition({ ScreenSize.x / 2.f, ScreenSize.y / 2.f });

		vec2f change = { newMouse.x - ScreenSize.x / 2.f, newMouse.y - ScreenSize.y / 2.f };

		camera.rotate(0, -rotation_speed * change.x, 0);
		camera.rotate(rotation_speed * change.y, 0, 0);
		camera.setLocation({ camera.getLocation().x, Chunk::getTerrain({camera.getLocation().x, camera.getLocation().z}) + 3.f, camera.getLocation().z });

		camera.setSpeed(2.5f);
		camera.pollEvents();

		Renderer::Render(Loader::getEntities(), camera);
		printThread.join();
	}
}