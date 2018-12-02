#pragma once

#include "Component.h"

namespace ECS
{
	class Baseless : public Component
	{
	public:

		virtual type getType() {
			return BASELESS;
		}
	};
}