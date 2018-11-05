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

		std::vector<unsigned int> indices = {
			0, 1, 2,
			2, 3, 0
		};

		model = Loader::makeModel(locations, indices);
	}

	void MainScreen::onUpdate() {
		if (Keyboard::isKeyPressed(KEY::ESCAPE))
			Window::close();

		Renderer::Render(model);

		printf((std::to_string(fps) + "\n").c_str());

		uFrame++;
	}

	void MainScreen::onDestroy() {
		Loader::destroy();
	}
}