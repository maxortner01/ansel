#include "../headers/FPS.h"

namespace Game
{
	FPS::FPS(Window* w) : Screen(w) {}

	void FPS::onCreate() {
		_rawModels.resize(AMOUNT);
		_models.resize(AMOUNT);

		_rawModels.at(WALL) = Loader::makeSquare();
		_rawModels.at(WALL)->loadTexture("assets/textures/wall.png");

		_rawModels.at(FLOOR) = Loader::makeSquare();
		_rawModels.at(FLOOR)->loadTexture("assets/textures/tile.png");

		_rawModels.at(CEILING) = Loader::makeSquare();
		_rawModels.at(CEILING)->loadTexture("assets/textures/ceiling.png");


		float scale = 4.f;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 2; j++) {
				if (j == 1) continue;
					
				Model* model = Loader::makeModel(_rawModels.at(WALL));
				model->setLocation({ (float)i * (2.f * scale), 0, 5.f + j * (2.f * scale)});
				model->setScale({ scale, scale, scale });

				//model->setRotation({ 0, 180.f * 3.14159f / 180.f * (-j + 1.f), 0 });

				_models.at(WALL).push_back(model);
			}

			Model* model = Loader::makeModel(_rawModels.at(FLOOR));
			model->setLocation({ (float)i * (2.f * scale), -scale, 5.f + (scale) });
			model->setScale({ scale, scale, scale });
			model->setRotation({ 270.f * 3.14159f / 180.f, 0, 0 });

			_models.at(FLOOR).push_back(model);

			model = Loader::makeModel(_rawModels.at(CEILING));
			model->setLocation({ (float)i * (2.f * scale), scale, 5.f + (scale) });
			model->setScale({ scale, scale, scale });
			model->setRotation({ 90.f * 3.14159f / 180.f, 0, 0 });

			_models.at(CEILING).push_back(model);
		}

		//Renderer::setLighting(false);

		Light light;
		light.on = true;
		//light.location = { 5.f, 2.f, 8.f };
		light.location = { 0, 1, 1 };
		light.type = light.DIRECTIONAL;

		Renderer::makeLight(light);

		camera.setLocation({ 0, 0, 7.f });
	}

	void FPS::onUpdate() {
		float rotation_speed = .1f * 3.14159f / 180.f;
		vec2d newMouse = Mouse::getPostion();
		Mouse::setPosition({ ScreenSize.x / 2.f, ScreenSize.y / 2.f });

		vec2f change = { newMouse.x - ScreenSize.x / 2.f, newMouse.y - ScreenSize.y / 2.f };

		camera.rotate(0, -rotation_speed * change.x, 0);
		camera.rotate(rotation_speed * change.y, 0, 0);

		camera.setSpeed(2.5f);
		camera.pollEvents();

		if (Keyboard::isKeyPressed(KEY::ESCAPE))
			window->close();

		if (Keyboard::isKeyPressed(KEY::LEFT)) {
			if (lights)
				lights = false;
			else
				lights = true;
		}

		Renderer::setLighting(lights);

		for (int i = 0; i < _models.size(); i++) {
			Renderer::Render(_models.at(i), camera);
		}
	}
}