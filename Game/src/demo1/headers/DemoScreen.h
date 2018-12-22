#pragma once

#include <vector>
#include <thread>
#include <future>
#include <Ansel.h>

namespace Demo1 {

	class DemoScreen : public Ansel::Screen
	{
		//std::vector<std::thread*> threads;
		//std::vector<std::promise<bool>*> futures;

		Ansel::Material* sand;

		Ansel::Camera camera;

	public:
		DemoScreen(Ansel::Window* w);

		void onUpdate(float timeDelta) override;
		void onCreate();
	};

}