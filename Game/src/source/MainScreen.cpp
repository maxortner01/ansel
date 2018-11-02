#include "../headers/MainScreen.h"

namespace Game
{
	MainScreen::MainScreen() : Screen() {}

	void MainScreen::onUpdate() {
		if (Ansel::Keyboard::isKeyPressed(Ansel::Keyboard::KEY::ESCAPE))
			Ansel::Window::close();

		draw.circle(location, 10.f, 100);
		draw.circle({ (float)Ansel::Mouse::getPostion().x, (float)Ansel::Mouse::getPostion().y }, 5.f);

		float speed = 2.f;
		if (Ansel::Keyboard::isKeyPressed(Ansel::Keyboard::KEY::W))
			location.y -= speed;

		if (Ansel::Keyboard::isKeyPressed(Ansel::Keyboard::KEY::A))
			location.x -= speed;

		if (Ansel::Keyboard::isKeyPressed(Ansel::Keyboard::KEY::S))
			location.y += speed;

		if (Ansel::Keyboard::isKeyPressed(Ansel::Keyboard::KEY::D))
			location.x += speed;

		/*
		Ansel::vec2f endpoint = { cosf((float)uFrame / 100.f), sinf((float)uFrame / 100.f) };

		Ansel::vec4f color = { 1, 0, 0, 1 };
		if (Ansel::Keyboard::isKeyPressed(Ansel::Keyboard::KEY::W))
			color = { 1, 1, 1, 1 };

		draw.rectangle({ -1, -1, 2, 2 }, { 1, 1, 1, 1 });
		draw.circle({ 0, 0 }, .5f, 100, color);

		if (uFrame % 100 == 0)
			printf((std::to_string(Ansel::Mouse::getPostion().x) + "\n").c_str());
		*/
		uFrame++;
	}
}