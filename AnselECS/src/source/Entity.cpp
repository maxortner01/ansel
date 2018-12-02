#include "../headers/Entity.h"
#include "../headers/EntityManager.h"

namespace ECS
{
	//void* Entity::operator new(size_t size) {
	//	return EntityManager::makeEntity();
	//}

	void Entity::addComponent(ComponentInstance comp) {
		components.push_back(comp);
	}
}