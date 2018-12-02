#include "../headers/EntityManager.h"

namespace ECS
{
	std::vector<EntityInstance> EntityManager::entities;

	EntityInstance EntityManager::makeEntity() {
		EntityInstance e = new Entity();
		entities.push_back(e);

		return e;
	}
}