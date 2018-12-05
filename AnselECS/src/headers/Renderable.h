#pragma once

#include "Component.h"

namespace ECS
{
	class Renderable : public Component
	{
	public:

		virtual int getType() {
			return RENDERABLE;
		}
	};
}