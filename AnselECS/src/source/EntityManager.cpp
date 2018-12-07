#include "../headers/EntityManager.h"

namespace ECS
{
	std::vector<std::string> EntityManager::names;
	std::vector<EntityInstance> EntityManager::entities;

	EntityInstance EntityManager::getEntity(const char* name) {
		int index = -1;
		for (int i = 0; i < entities.size(); i++)
			if (names.at(i) == name) {
				index = i; break;
			}

		if (index == -1) return nullptr;

		return entities.at(index);
	}

	EntityInstance EntityManager::makeEntity(const char* name) {

		EntityInstance e = new Entity();
		entities.push_back(e);
		names.push_back(name);

		return e;
	}

	std::vector<EntityInstance> EntityManager::getEntities() {
		return entities;
	}
}