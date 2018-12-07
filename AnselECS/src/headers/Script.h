#pragma once

#include "Baseless.h"

namespace ECS
{
	/**
	  * Empty Script object that contains one pure virtual function: update()
	  */
	class Script : public Baseless
	{
	public:
		virtual void update() = 0;

		virtual int getType() {
			return SCRIPT;
		}
	};
}