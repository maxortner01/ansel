#pragma once

#include <Ansel.h>

namespace Game {
	class MainScreen : public Ansel::Screen
	{
		Ansel::vec2f location = { 0, 0 };

	public:
		MainScreen();

		void onUpdate();
	};
}