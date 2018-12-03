#include "../headers/Entity.h"
#include "../headers/EntityManager.h"

namespace ECS
{
	//void* Entity::operator new(size_t size) {
	//	return EntityManager::makeEntity();
	//}

	void Entity::addComponent(ComponentInstance comp, const char* name) {
		components.push_back(comp);
		names.push_back(name);
	}

	ComponentInstance Entity::getComponent(const char* component_name) {
		if (component_name == "") return nullptr;

		int index = -1;
		for (int i = 0; i < components.size(); i++)
			if (names.at(i) == component_name) {
				index = i; break;
			}

		if (index == -1) return nullptr;

		return components.at(index);
	}
}