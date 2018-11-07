#include "../headers/MainScreen.h"

namespace Game
{
	Asteroid::Asteroid(RawModel* rawModel) : Model(rawModel) {
		const int   max  = 1000;
		const float prec = (float)max / 100.f;

		angle = (rand() % 360000) / 1000.f;
		radius = 100.f + ((rand() % max) / prec - ((float)max / prec) / 2.f);
	}

	void Asteroid::update() {
		setLocation(radius * cosf(angle * 180.f / 3.14159 / radius), 0, radius * sinf(angle * 180.f / 3.14159 / radius), 1);
		angle += .001f;
	}

	MainScreen::MainScreen(Window* window) : Screen(window) {
		std::vector<vec3f> locations = {
			{ -1.f,  1.f, 1.f },
			{ -1.f, -1.f, 1.f },
			{  1.f, -1.f, 1.f },
			{  1.f,  1.f, 1.f },
		};

		std::vector<vec4f> colors = {
			{ 0, 0, 1, 1 },
			{ 1, 0, 0, 1 },
			{ 0, 1, 0, 1 },
			{ 1, 1, 1, 1 }
		};

		//for (vec3f &v : locations)
		//	v = { v.x / 10.f, v.y / 10.f, v.z / 10.f };

		std::vector<unsigned int> indices = {
			0, 1, 2,
			2, 3, 0
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
	}

	void MainScreen::onUpdate() {
		if (Keyboard::isKeyPressed(KEY::ESCAPE))
			Window::close();

		float speed = .1f;

		if (Keyboard::isKeyPressed(KEY::L_SHIFT))
			speed *= 2;

		if (Keyboard::isKeyPressed(KEY::W))
			camera.translate(0, 0, speed);

		if (Keyboard::isKeyPressed(KEY::S))
			camera.translate(0, 0, -speed);

		if (Keyboard::isKeyPressed(KEY::D))
			camera.translate(speed, 0, 0);

		if (Keyboard::isKeyPressed(KEY::A))
			camera.translate(-speed, 0, 0);

		if (Keyboard::isKeyPressed(KEY::SPACE))
			camera.translate(0, speed, 0);

		if (Keyboard::isKeyPressed(KEY::L_CTRL))
			camera.translate(0, -speed, 0);

		//models.at(0)->setLocation(sinf(uFrame / 100.f) / 2.f, cosf(uFrame / 100.f) / 2.f);
		//models.at(0)->addLocation(0, 0, .01f, 0);

		//for (int y = 0; y < 100; y++) {
		//	for (int x = 0; x < 100; x++) {
		//		Model* model = models.at(y * 100 + x);

		//		model->addLocation(0, sinf(x + (float)uFrame / 100.f) / 100.f, 0, 0);
		//	}
		//}

		for (int i = 0; i < models.size(); i++)
			models.at(i)->update();

		Renderer::Render(models, camera);

		printf((std::to_string(getFPS()) + "\n").c_str());

		uFrame++;
	}

	void MainScreen::onDestroy() {

	}
}