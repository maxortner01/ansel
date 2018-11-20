#pragma once

#include <Ansel.h>
#include <vector>

using namespace Ansel;

namespace Game
{
	class LightTest : public Screen
	{
		RawModel* cube;
		std::vector<Model*> cubes;

		Camera    camera;

	public:
		LightTest(Window* w);

		void onCreate() override;
		void onUpdate() override;
	};


}