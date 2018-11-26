#pragma once

#include "def.h"

namespace Alloc
{
	class Allocator
	{
	public:
		virtual void* allocate(size_t size) = 0;
		virtual void  free(void* ptr) = 0;
	};
}