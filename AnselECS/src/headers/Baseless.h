#pragma once

#include "Component.h"

namespace ECS
{
	class Baseless : public Component
	{
	public:

		virtual int getType() {
			return BASELESS;
		}
	};
}