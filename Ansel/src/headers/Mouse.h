#pragma once

#include "Def.h"
#include "util.h"
#include "Window.h"
#include <GLFW/glfw3.h>

namespace Ansel
{
	struct ANSEL_API Mouse
	{
		static vec2d getPostion(bool off_screen = false);
		static void  setPosition(vec2f position);

		static void  hideCursor();
	};
}