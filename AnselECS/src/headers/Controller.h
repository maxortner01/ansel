#pragma once

#include "Baseless.h"

namespace ECS
{
	class Controller : public Baseless
	{
	public:

		virtual void getEntity() {}

		virtual type getType() {
			return CONTROLLER;
		}
	};
}