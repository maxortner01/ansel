#pragma once

#include "def.h"

namespace Alloc
{
	template<typename T>
	class Allocator
	{
		struct _BLOCK
		{
			T val;
			_BLOCK* next;
		};

		static unsigned int counter;

	public:
		inline static int getValue() {
			return counter++;
		}
	};

	template<typename T>
	unsigned int Allocator<T>::counter = 0;
}