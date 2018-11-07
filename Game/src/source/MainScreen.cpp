#include "../headers/MainScreen.h"

namespace Game
{
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

		models.push_back(new Model(rawModel));
	}

	void MainScreen::onUpdate() {
		if (Keyboard::isKeyPressed(KEY::ESCAPE))
			Window::close();

		float speed = .01f;

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

		//models.at(0)->setLocation(sinf(uFrame / 100.f) / 2.f, cosf(uFrame / 100.f) / 2.f);
		//models.at(0)->addLocation(0, 0, .01f, 0);

		Renderer::Render(models, camera);

		printf((std::to_string(getFPS()) + "\n").c_str());

		uFrame++;
	}

	void MainScreen::onDestroy() {

	}
}