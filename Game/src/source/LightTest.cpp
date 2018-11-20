#include "../headers/LightTest.h"

#include <cstdlib>
#include <iostream>

namespace Game
{
	LightTest::LightTest(Window* w) : Screen(w) {}

	void LightTest::onCreate() {
		// Generate cube model
		std::vector<vec3f> vertices = {
			{ -1.f, -1.f, -1.f },
			{  1.f,  1.f, -1.f },
			{  1.f, -1.f, -1.f },
			{ -1.f,  1.f, -1.f },

			{ -1.f, -1.f,  1.f },
			{  1.f,  1.f,  1.f },
			{  1.f, -1.f,  1.f },
			{ -1.f,  1.f,  1.f },
		};

		std::vector<unsigned int> indices = {
			// Back face
			0, 1, 2, 0, 3, 1,
			// Front face
			4, 5, 6, 4, 7, 5,
			// Bottom face
			0, 6, 2, 0, 4, 6
		};

		std::vector<vec4f> colors;
		for (int i = 0; i < vertices.size(); i++)
			colors.push_back({ 1, 1, 1, 1 });

		cube = Loader::makeRawModel(vertices, indices);
		cube->loadColors(colors);

		float space = 4.f;
		for (int x = -1; x < 2; x++) {
			for (int y = -1; y < 2; y++) {
				for (int z = -1; z < 2; z++) {
					Model* c = new Model(cube);

					c->setLocation({ x * space, y * space, z * space });

					cubes.push_back(c);
				}
			}
		}

		Renderer::setLighting(false);
		camera.setSpeed(.5f);
	}

	void LightTest::onUpdate() {
		if (Keyboard::isKeyPressed(KEY::ESCAPE))
			window->close();

		camera.pollEvents();

		float rotation_speed = .1f * 3.14159f / 180.f;
		vec2d newMouse = Mouse::getPostion();
		Mouse::setPosition({ ScreenSize.x / 2.f, ScreenSize.y / 2.f });

		vec2f change = { newMouse.x - ScreenSize.x / 2.f, newMouse.y - ScreenSize.y / 2.f };

		camera.rotate(0, -rotation_speed * change.x, 0);
		camera.rotate(rotation_speed * change.y, 0, 0);



		Renderer::Render(cubes, camera);
	}
}