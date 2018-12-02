#pragma once

#include <vector>

#include "def.h"
#include "Component.h"

namespace ECS
{
	typedef std::vector<ComponentInstance> Components;

	class Entity
	{
		Components components;

	public:
		//void ECS_API * operator new(size_t size);

		void ECS_API addComponent(ComponentInstance comp);

		Components getComponents() { return components; }
	};

	using EntityInstance = Entity * ;
}