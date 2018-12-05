#pragma once

#include <Ansel.h>

namespace Game
{
	class Object : public ECS::Script
	{
		float health = 1.f;

	public:
		void update() {
			health -= .00001f;
		}

		float getValue() {
			return health;
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