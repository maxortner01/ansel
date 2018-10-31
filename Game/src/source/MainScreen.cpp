#include "../headers/MainScreen.h"

namespace Game
{
	MainScreen::MainScreen() : Screen() {

	}

	void MainScreen::onUpdate() {
		draw.line({ 0, 0 }, { cosf((float)uFrame / 100.f), sinf((float)uFrame / 100.f) });

		uFrame++;
	}
}