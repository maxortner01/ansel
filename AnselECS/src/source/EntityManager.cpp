#include "../headers/EntityManager.h"

namespace ECS
{
	std::vector<IEntity*> EntityManager::entities;

	template<typename T, typename... Args>
	EntityInt EntityManager::CreateEntity(Args... args) {
		IEntity* address = new Entity<T>(args);

		entities.push_back(address);

		return address->getEntityID();
	}

	void EntityManager::DestroyEntity(EntityInt ID) {
		delete entities.at(ID);
	}

	IEntity* EntityManager::getEntity(EntityInt ID) {
		return entities.at(ID);
	}
}