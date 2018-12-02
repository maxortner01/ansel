#pragma once

#include "def.h"
#include "IndexedManager.h"

#include <cstdlib>
#include <bitset>

namespace Alloc
{
	template<typename T>
	class IndexedAllocator
	{
		static bool made;
		static void* _ptr;

		static std::bitset<BITS> _indexes;

	public:
		static inline void makeNew(const unsigned int amount) {
			_ptr = IndexedManager<T>::makeBlock(amount);
			
			made = true;
		}

		static inline void* allocate() {

		}

		static inline void free(void* ptr) {

		}
	};

	template<typename T>
	void* IndexedAllocator<T>::_ptr = nullptr;

	template<typename T>
	std::bitset<BITS> IndexedAllocator<T>::_indexes;

	template<typename T>
	bool IndexedAllocator<T>::made = false;
}