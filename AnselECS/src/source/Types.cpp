#include "../headers/Types.h"

namespace ECS
{
	template<typename T>
	EntityInt EntityID::value() {
		static EntityInt id = count++;
		return id;
	}
}