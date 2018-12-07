#pragma once

#include <vector>

#include "def.h"
#include "Entity.h"

namespace ECS
{
	class EntityManager
	{
		static std::vector<std::string> names;
		static std::vector<EntityInstance> entities;

	public:
		static EntityInstance ECS_API getEntity(const char* name);
		static EntityInstance ECS_API makeEntity(const char* name = "");
		static std::vector<EntityInstance> ECS_API getEntities();
	};
}