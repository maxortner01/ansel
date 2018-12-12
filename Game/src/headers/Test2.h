#pragma once

#include <Ansel.h>

using namespace Ansel;

namespace Game
{
	class Mover : public ECS::Controller
	{
		bool moving = false;
		unsigned int ID = 0;
		Camera* player;

	public:
		Mover(Camera* camera);

		void update(ECS::EntityInstance entity);
	};

	class Test2 : public Screen
	{
		Camera camera;

	protected:
		void onUpdate(float timeDelta);
		void onCreate();

	public:
		Test2(Window* w);
	};
}