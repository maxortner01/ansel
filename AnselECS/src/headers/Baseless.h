#pragma once

#include "Component.h"

namespace ECS
{
	class Baseless : public Component
	{
	protected:
		bool _update = false;

	public:

		virtual int getType() {
			return BASELESS;
		}

		bool updateable() { 
			return _update; 
		}
	};
}