#pragma once

#include <Ansel.h>

using namespace Ansel;

namespace Game {
	class MainScreen : public Ansel::Screen
	{
		Model model;

		Engine* engine;

	public:
		MainScreen(Window* window);

		void onDestroy() override;
		void onUpdate() override;
	};
}