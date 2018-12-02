#pragma once

#include <vector>

#include "def.h"
#include "Entity.h"

namespace ECS
{
	class EntityManager
	{
		static std::vector<EntityInstance> entities;

	public:

		static EntityInstance ECS_API makeEntity();
	};
}