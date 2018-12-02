#pragma once

#include <Ansel.h>

namespace Game
{
	class Object : public ECS::Script
	{
	public:
		void update() {

		}
	};

	class GameScreen : public Ansel::Screen
	{
	protected:
		void onUpdate(float timeDelta) override;
		void onCreate() override;

	public:
		GameScreen(Ansel::Window* w);
	};
}