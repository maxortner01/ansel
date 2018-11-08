#include "../headers/MainScreen.h"

namespace Game
{
	Asteroid::Asteroid(RawModel* rawModel) : Model(rawModel) {
		const int   max  = 1000;
		const float prec = (float)max / 100.f;

		rotation_rates = { 
			(rand() % 360000) / 100000.f * 3.14159f / 180.f,
			(rand() % 360000) / 100000.f * 3.14159f / 180.f,
			(rand() % 360000) / 100000.f * 3.14159f / 180.f
		};

		angle = (rand() % 360000) / 1000.f;
		radius = 100.f + ((rand() % max) / prec - ((float)max / prec) / 2.f);
		
		float scale_factor = (rand() % 10) / 10.f;
		setScale({ scale_factor, scale_factor, scale_factor });
	}

	void Asteroid::update() {
		setLocation(radius * cosf(angle * 180.f / 3.14159 / radius), 0, radius * sinf(angle * 180.f / 3.14159 / radius), 1);
		angle += .001f;

		addRotation(rotation_rates.x, rotation_rates.y, rotation_rates.z);
	}

	MainScreen::MainScreen(Window* window) : Screen(window) {
		std::vector<vec3f> locations = {
			{ -1.f,  1.f, 1.f },
			{ -1.f, -1.f, 1.f },
			{  1.f, -1.f, 1.f },
			{  1.f,  1.f, 1.f },

			{ -1.f,  1.f, -1.f },
			{ -1.f, -1.f, -1.f },
			{  1.f, -1.f, -1.f },
			{  1.f,  1.f, -1.f },
		};

		std::vector<vec4f> colors = {
			{ 0, 0, 1, 1 },
			{ 1, 0, 0, 1 },
			{ 0, 1, 0, 1 },
			{ 1, 1, 1, 1 },

			{ 0, 0, 1, 1 },
			{ 1, 0, 0, 1 },
			{ 0, 1, 0, 1 },
			{ 1, 1, 1, 1 },
		};

		//for (vec3f &v : locations)
		//	v = { v.x / 10.f, v.y / 10.f, v.z / 10.f };

		std::vector<unsigned int> indices = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			3, 2, 6,
			6, 7, 3,

			0, 1, 5,
			5, 4, 0,

			0, 4, 7,
			7, 3, 0,

			1, 5, 6,
			6, 2, 1
		};

		rawModel = Loader::makeRawModel(locations, indices);
		rawModel->loadColors(colors);

		int range = 50;
		for (int y = 0; y < range; y++) {
			for (int x = 0; x < range; x++) {
				Model* model = new Asteroid(rawModel);

				models.push_back(model);
			}
		}

		Mouse::hideCursor();
	}

	void MainScreen::onUpdate() {
		if (Keyboard::isKeyPressed(KEY::ESCAPE))
			Window::close();

		float rotation_speed = .1f * 3.14159f / 180.f;
		float speed = .1f;

		if (Keyboard::isKeyPressed(KEY::L_SHIFT))
			speed *= 2;

		if (Keyboard::isKeyPressed(KEY::W))
			camera.move(Camera::FORWARDS, speed);

		if (Keyboard::isKeyPressed(KEY::S))
			camera.move(Camera::BACKWARDS, speed);

		if (Keyboard::isKeyPressed(KEY::D))
			camera.move(Camera::RIGHT, speed);

		if (Keyboard::isKeyPressed(KEY::A))
			camera.move(Camera::LEFT, speed);

		if (Keyboard::isKeyPressed(KEY::SPACE))
			camera.translate(0, speed, 0);

		if (Keyboard::isKeyPressed(KEY::L_CTRL))
			camera.translate(0, -speed, 0);

		if (Keyboard::isKeyPressed(KEY::UP))
			camera.rotate(-rotation_speed, 0, 0);

		if (Keyboard::isKeyPressed(KEY::DOWN))
			camera.rotate(rotation_speed, 0, 0);

		if (Keyboard::isKeyPressed(KEY::LEFT))
			camera.rotate(0, rotation_speed, 0);

		if (Keyboard::isKeyPressed(KEY::RIGHT))
			camera.rotate(0, -rotation_speed, 0);

		vec2d newMouse = Mouse::getPostion();
		Mouse::setPosition({ ScreenSize.x / 2.f, ScreenSize.y / 2.f });

		vec2f change = { newMouse.x - ScreenSize.x / 2.f, newMouse.y - ScreenSize.y / 2.f };
		
		camera.rotate(0, -rotation_speed * change.x, 0);
		camera.rotate(rotation_speed * change.y, 0, 0);

		for (int i = 0; i < models.size(); i++)
			models.at(i)->update();

		Renderer::Render(models, camera);

		printf((std::to_string(getFPS()) + "\n").c_str());

		uFrame++;
	}

	void MainScreen::onDestroy() {

	}
}