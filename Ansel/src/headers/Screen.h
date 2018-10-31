#pragma once

#include "Def.h"
#include "util.h"

#include <GLFW/glfw3.h>

namespace Ansel
{
	class DECLSPECIFIER Screen
	{
	protected:
		struct DECLSPECIFIER Draw {
			Draw();

			void line(Ansel::vec2f startpoint, Ansel::vec2f endpoint);
		};

		Draw draw;
		unsigned int uFrame = 0;

	public:
		Screen();

		virtual void onUpdate() = 0;

		virtual void onCreate();
		virtual void onDestroy();
	};
}