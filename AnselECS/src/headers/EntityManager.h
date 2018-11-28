#pragma once

#include "def.h"
#include "Entity.h"

#include <vector>

namespace ECS
{
	class EntityManager
	{
		static std::vector<IEntity*> entities;

	public:
		template<typename T, typename... Args>
		static EntityInt CreateEntity(Args... args);

		static void DestroyEntity(EntityInt ID);
		static IEntity* getEntity(EntityInt ID);
	};
}