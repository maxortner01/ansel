#include "../headers/MainScreen.h"

namespace Game
{
	MainScreen::MainScreen(Window* window) : Screen(window) {
		std::vector<vec2f> locations = {
			{ -1.f,  1.f },
			{ -1.f, -1.f },
			{  1.f, -1.f },
			{  1.f,  1.f },
		};

		std::vector<vec4f> colors = {
			{ 0, 0, 1, 1 },
			{ 0, 1, 0, 1 },
			{ 1, 0, 0, 1 }
		};

		for (vec2f &v : locations)
			v = { v.x / 2.f, v.y / 2.f };

		std::vector<unsigned int> indices = {
			0, 1, 2,
			2, 3, 0
		};

		rawModel = Loader::makeModel(locations, indices);
		//rawModel->loadColor(colors);

		model    = Model(rawModel);
	}

	void MainScreen::onUpdate() {
		if (Keyboard::isKeyPressed(KEY::ESCAPE))
			Window::close();

		Renderer::Render(model);

		//printf((std::to_string(fps) + "\n").c_str());

		uFrame++;
	}

	void MainScreen::onDestroy() {
		Loader::destroy();
	}
}