#pragma once

#include <Ansel.h>

using namespace Ansel;

namespace Game
{
	class Test2 : public Screen
	{
	protected:
		void onUpdate(float timeDelta);
		void onCreate();

	public:
		Test2(Window* w);
	};
}