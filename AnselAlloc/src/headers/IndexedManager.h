#pragma once

#include "def.h"

#include <cstdlib>
#include <bitset>

namespace Alloc
{
	template<typename T>
	class IndexedManager
	{
		static const int indexSize;

		static void* _ptr;
		static bool  made;

		static int*  _indexPtr;

		static inline void make() {
			// Allocate 8 * 1000 bytes for a thousand pointers
			_ptr = std::malloc(sizeof(int) * 1000 + indexSize);

			// Iterate through the first bits
			// size = ceil(1000 / 64)
			char* bPtr = (char*)_ptr;
			for (int i = 0; i < indexSize; i++) {
				std::bitset<BITS> bits;
				*(bPtr + i) = bits.to_ullong();
			}

			// Set all the 8-byte slots to 0
			_indexPtr = (int*)((char*)(_ptr)+ indexSize);
			for (int i = 0; i < 1000; i++)
				*(_indexPtr + i) = (uintptr_t)0;

			made = true;
		}

	public:
		static inline void* getBlock(const unsigned int amount) {
			if (!made) make(); 

			int index = -1;
			// Look for first empty pointer
			for (int i = 0; i < indexSize; i++) {
				// Read the byte into the bitset
				std::bitset<BITS> indexes(*((char*)_ptr + i));

				// Iterate through the bits
				for (int j = 0; j < BITS; j++) {
					// Read the bit at j
					bool b = indexes[j];
					
					// If the bit is 0, the index is free
					if (!b) {
						// Occupy the space in the bitset and rewrite to memory
						indexes[j] = 1;
						*((char*)_ptr + i) = indexes.to_ullong();

						index = (i * BITS) + j;
						break;
					}

				}

				if (index != -1) break;
			}

			if (index == -1) return nullptr;

			return _indexPtr + index;
		}
	};

	template<typename T>
	bool IndexedManager<T>::made = false;

	template<typename T>
	void* IndexedManager<T>::_ptr = nullptr;

	template<typename T>
	int* IndexedManager<T>::_indexPtr = nullptr;

	template<typename T>
	const int IndexedManager<T>::indexSize = std::ceil(1000.f / (float)BITS);
}