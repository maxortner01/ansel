#pragma once

#include "Def.h"
#include "Window.h"

#include <vector>
#include <GLFW/glfw3.h>

namespace Ansel
{
	enum STATE {
		PRESS = GLFW_PRESS,
		RELEASE = GLFW_RELEASE,
		NONE
	} typedef STATE;

	enum KEYS {
		W = GLFW_KEY_W,
		A = GLFW_KEY_A,
		S = GLFW_KEY_S,
		D = GLFW_KEY_D,
		ESCAPE = GLFW_KEY_ESCAPE,
		L_SHIFT = GLFW_KEY_LEFT_SHIFT,
		L_CTRL = GLFW_KEY_LEFT_CONTROL,
		SPACE = GLFW_KEY_SPACE,
		UP    = GLFW_KEY_UP,
		DOWN  = GLFW_KEY_DOWN,
		LEFT  = GLFW_KEY_LEFT,
		RIGHT = GLFW_KEY_RIGHT,
		TAB   = GLFW_KEY_TAB,
		AMT = 127
	} typedef KEY;

	class ANSEL_API Keyboard
	{
	public:
		static bool isKeyPressed(KEY key);
	};
}