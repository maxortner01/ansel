#include "../headers/Entity.h"

namespace ECS
{
	const EntityInt IEntity::getEntityID() const { return ID; }

	IEntity::IEntity() {
		ID++;
	}

	IEntity::~IEntity() {}

	template<typename T>
	Entity<T>::Entity() {
		typeID = EntityID::value<T>();
	}

	template<typename T>
	Entity<T>::~Entity() { }

	template<typename T>
	const EntityInt Entity<T>::getTypeID() const {
		return typeID;
	}
}