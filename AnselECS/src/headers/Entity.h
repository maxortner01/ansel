#pragma once

#include "def.h"
#include "Types.h"

namespace ECS
{
	class IEntity
	{
		EntityInt ID;

	public:
		IEntity();
		virtual ~IEntity();

		const EntityInt getEntityID() const;
		virtual const EntityInt getTypeID() const = 0;
	};

	template<typename T>
	class Entity : public IEntity
	{
		const EntityID typeID;

		//void operator delete(void*) = delete;
		//void operator delete[](void*) = delete;

	public:
		ECS_API Entity();
		virtual ECS_API ~Entity();

		const EntityInt ECS_API getTypeID() const;
	};
}