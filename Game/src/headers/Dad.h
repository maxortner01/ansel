#pragma once

#include <Ansel.h>

using namespace Ansel;

namespace Game
{
	class Dad : public Screen
	{
	public:
		Dad(Window* w);

		void onUpdate(float timeDelta);
		void onCreate();
	};
}