#pragma once

#include "def.h"

namespace ECS
{
	class EntityID
	{
		static EntityInt count;

	public:
		template<typename T>
		static EntityInt value();
	};
}