#pragma once

#include <Ansel.h>

using namespace Ansel;

namespace Game {
	class MainScreen : public Ansel::Screen
	{
		RawModel* rawModel;
		
		std::vector<Model*> models;

		Engine* engine;
		Camera camera;

	public:
		MainScreen(Window* window);

		void onDestroy() override;
		void onUpdate() override;
	};
}