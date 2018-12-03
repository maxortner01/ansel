#pragma once

#include <vector>

#include "def.h"
#include "Component.h"

namespace ECS
{
	typedef std::vector<ComponentInstance> Components;

	class Entity
	{
		std::vector<const char*> names;
		Components components;

	public:
		//void ECS_API * operator new(size_t size);

		void ECS_API addComponent(ComponentInstance comp, const char* name = "");

		ComponentInstance ECS_API getComponent(const char* component_name);
		Components getComponents() { return components; }
	};

	using EntityInstance = Entity * ;
}