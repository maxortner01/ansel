#pragma once

#include <Ansel.h>

using namespace Ansel;

namespace Game
{
	class Terrain : public Screen
	{
		Camera camera;

	protected:
		void onUpdate(float timeDelta);
		void onCreate();

	public:
		Terrain(Window* w);
	};
}