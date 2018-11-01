#include "../headers/MainScreen.h"

namespace Game
{
	MainScreen::MainScreen() : Screen() {}

	void MainScreen::onUpdate() {
		Ansel::vec2f endpoint = { cosf((float)uFrame / 100.f), sinf((float)uFrame / 100.f) };

		draw.rectangle({ -1, -1, 2, 2 }, { 1, 1, 1, 1 });
		draw.circle({ 0, 0 }, .5f, 100, { 1, 0, 0, 1 });
		//draw.line({ 0, 0 }, endpoint, { 0, 0, 0 });
		
		uFrame++;
	}
}