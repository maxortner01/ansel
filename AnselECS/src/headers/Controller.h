#pragma once

#include "Baseless.h"

namespace ECS
{
	class Controller : public Baseless
	{
	public:

		virtual void update(ECS::Entity* entity) {}

		virtual int getType() {
			return CONTROLLER;
		}
	};
}