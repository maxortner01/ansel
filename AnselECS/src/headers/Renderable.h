#pragma once

#include "Component.h"

namespace ECS
{
	class Renderable : public Component
	{
	public:

		virtual type getType() {
			return RENDERABLE;
		}
	};
}