#pragma once

#include <Ansel.h>

using namespace Ansel;

namespace Game {
	
	class Asteroid : public Ansel::Model
	{
		float angle = 0;
		float radius = 0;

	public:
		Asteroid(RawModel* rawModel);

		void update() override;
	};

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