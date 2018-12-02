#pragma once

#include <Ansel.h>

using namespace Ansel;

namespace Game
{
	class FPS : public Screen
	{
		enum RawModels
		{
			WALL,
			FLOOR,
			CEILING,
			AMOUNT
		};

		std::vector<RawModel*> _rawModels;
		std::vector<std::vector<Model*>> _models;
		Camera camera;

		bool lights = true;

	public:
		FPS(Window* w);

		void onCreate() override;
		void onUpdate() override;
	};
}