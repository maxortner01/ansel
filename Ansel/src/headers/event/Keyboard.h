#pragma once

#include "../Def.h"
#include "../Window.h"

namespace Ansel
{
	enum KEYS {
		W,      
		A,      
		S,      
		D,      
		ESCAPE,
		L_SHIFT,
		L_CTRL, 
		SPACE,  
		UP,     
		DOWN,   
		LEFT,   
		RIGHT,  
		TAB,   
		AMT = 127
	} typedef KEY;

	class ANSEL_API Keyboard
	{
	public:
		static bool isKeyPressed(KEY key);
	};
}