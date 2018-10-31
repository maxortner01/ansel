#pragma once

#include <Ansel.h>

namespace Game {
	class MainScreen : public Ansel::Screen
	{
	public:
		MainScreen();

		void onUpdate();
	};
}