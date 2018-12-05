#pragma once

#include "Baseless.h"

namespace ECS
{
	/**
	  * Empty Script object that contains one pure virtual function: update()
	  */
	class Script : public Baseless
	{
		bool _update = false;

	public:
		virtual void update() = 0;

		virtual int getType() {
			return SCRIPT;
		}

		bool updateable() { return _update; }
	};
}