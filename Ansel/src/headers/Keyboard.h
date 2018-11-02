#pragma once

#include "Def.h"
#include "Window.h"

#include <vector>
#include <GLFW/glfw3.h>

namespace Ansel
{
	class ANSEL_API Keyboard
	{
	public:
		enum STATE {
			PRESS   = GLFW_PRESS,
			RELEASE = GLFW_RELEASE,
			NONE
		} typedef STATE;

		enum KEYS {
			W      = GLFW_KEY_W,
			A	   = GLFW_KEY_A,
			S	   = GLFW_KEY_S,
			D	   = GLFW_KEY_D,
			ESCAPE = GLFW_KEY_ESCAPE,
			AMT = 127
		} typedef KEY;

		static bool isKeyPressed(KEY key);
	};
}