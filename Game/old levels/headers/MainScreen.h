#pragma once

#include <Ansel.h>
#include <iostream>
#include <cstdlib>

using namespace Ansel;

namespace Game {
	
	class Asteroid : public Ansel::Model
	{
		vec3f rotation_rates;

		float angle = 0;
		float radius = 0;

	public:
		Asteroid(RawModel* rawModel, const int max = 250, const int min = 50);

		void update() override;
	};

	class MainScreen : public Ansel::Screen
	{
		RawModel* rawModel;
		RawModel* rawPlanet;
		
		std::vector<Model*> models;
		Model* planetModel;

		Engine* engine;
		Camera camera;

	public:
		MainScreen(Window* window);

		void onDestroy() override;
		void onUpdate() override;
	};
}