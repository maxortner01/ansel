#include <Ansel.h>
#include <iostream>
#include <ctime>
#include <string>

#include "headers/MainScreen.h"
#include "headers/Terrain.h"
#include "headers/Textures.h"
#include "headers/LightTest.h"
/*
struct _BLOCK {
	int value;
	_BLOCK* next;
};

class Allocator
{
	const unsigned int amount = 400;
	unsigned int counter = 0;

	void*   ptr;
	_BLOCK* _BLOCK_PTR;

public:

	void init() {
		ptr = std::malloc(sizeof(_BLOCK) * amount + amount);

		_BLOCK_PTR = (_BLOCK*)((char*)ptr + amount);

		for (int i = 0; i < amount; i++) {
			*((char*)ptr + i) = 0;
		}
	}

	void make(_BLOCK block) {
		block.next = _BLOCK_PTR + counter + 1;
		*(_BLOCK_PTR + counter  ) = block;
		*((char*)ptr + counter++) = 1;
	}

	void free() {
		std::free(ptr);
	}

	void print() {
		for (int i = 0; i < amount; i++) {
			std::cout << (uintptr_t)(char*)(ptr)+1 << "   " << (int)*((char*)(ptr)+i) << std::endl;
		}

		std::cout << std::endl;

		for (int i = amount; i < sizeof(_BLOCK) * amount / sizeof(char); i++) {
			std::cout << (uintptr_t)(char*)(ptr)+i << "   " << (int)*((char*)(ptr)+i) << std::endl;
			if ((i - amount) % sizeof(_BLOCK) == sizeof(_BLOCK) - 1) {
				std::cout << std::endl;
			}
		}
	}
	
}; */

template<typename T>
struct _BLOCK {
	T val;
	_BLOCK* next;
};

template<typename T>
class MemoryManager
{
	static const unsigned int amount = 3000;

	// Pointer to the beginning of this section
	static void*   _ptr;
	// Pointer to the first _BLOCK
	static _BLOCK<T>* _BLOCK_PTR;

public:
	static void init() {
		// Initialize pointer to beginning of section
		_ptr = std::malloc(amount * sizeof(T));

		// Write indices
		for (int i = 0; i < amount; i++)
			*((char*)_ptr + i) = 0;

		// Initialize _BLOCK_PTR 
		_BLOCK_PTR = (_BLOCK<T>*)((char*)_ptr + amount);
	}

	static T* allocate() {
		int index = -1;
		int next  = -1;
		for (int i = 0; i < amount; i++) {
			int it = *((char*)_ptr + i);

			bool b = (bool)*((char*)_ptr + i);
			/**/ if ( !b && index == -1 ) {
				index = i; continue;
			}
			else if ( !b && index != -1 ) {
				next = i; break;
			}
		}

		if (index == -1 || next == -1) return nullptr;

		_BLOCK<T>* block = _BLOCK_PTR + index;
		block->next = _BLOCK_PTR + next;

		*((char*)_ptr + index) = 1;

		return &block->val;
	}

	static void free(void* ptr) {
		int index = std::floor(((uintptr_t)ptr - (uintptr_t)_BLOCK_PTR) / sizeof(T));
		*((char*)_ptr + index) = 0;
	}

	static void free_all() {
		std::free(_ptr);
	}

	static void print() {
		for (int i = 0; i < amount; i++) {
			std::cout << (uintptr_t)((char*)_ptr + i) << "   " << (int)*((char*)_ptr + i) << std::endl;
		}

		std::cout << std::endl;

		for (int i = amount; i < sizeof(_BLOCK<T>) * amount / sizeof(char); i++) {
			std::cout << (uintptr_t)(char*)(_ptr)+i << "   " << (int)*((char*)(_ptr)+i) << std::endl;
			if ((i - amount) % sizeof(_BLOCK<T>) == sizeof(_BLOCK<T>) - 1) {
				std::cout << std::endl;
			}
		}
	}
};

template<typename T>
_BLOCK<T>* MemoryManager<T>::_BLOCK_PTR = nullptr;

template<typename T>
void* MemoryManager<T>::_ptr = nullptr;

int main()
{
	/*
	int test_amt = 200;
	std::clock_t clock = std::clock();

	Allocator allocator;
	allocator.init();

	for (int i = 0; i < test_amt; i++)
		allocator.make({ i * 2 + 2 });
	
	double t = 1000.0 * (std::clock() - clock) / CLOCKS_PER_SEC;

	allocator.print();

	std::cout << "Finished in " << t << " milliseconds." << std::endl;

	allocator.free();*/

	struct Person
	{
		const char* name;
		int age;
	};

	clock_t clock = std::clock();

	Person* people[1000];
	for (int i = 0; i < 5000; i++) {
		for (int j = 0; j < 1000; j++) {
			people[j] = new Person;
		}

		for (int j = 0; j < 1000; j++) {
			delete people[j];
		}
	}

	std::cout << "It took " << 1000.0 * (std::clock() - clock) / CLOCKS_PER_SEC / 1000.0 << " seconds." << std::endl;
	std::getchar();

	MemoryManager<Person>::init();
	clock = std::clock();

	for (int i = 0; i < 5000; i++) {
		for (int j = 0; j < 1000; j++) {
			people[j] = MemoryManager<Person>::allocate();
		}

		for (int j = 0; j < 1000; j++) {
			MemoryManager<Person>::free(people[j]);
		}
	}

	std::cout << "It took " << 1000.0 * (std::clock() - clock) / CLOCKS_PER_SEC / 1000.0 << " seconds." << std::endl;
	std::getchar();
	return 0;

	Person* p = MemoryManager<Person>::allocate();
	p->name = "Max";
	p->age = 18;

	MemoryManager<Person>::print();

	std::getchar();

	MemoryManager<int>::free_all();

	// Window
	Ansel::Window window(2560, 1440);
	// Screen instance
	Game::LightTest screen(&window);
	// Settings for engine
	Ansel::Settings settings;

	Ansel::Engine engine(&window, &screen, settings);
	engine.run();
}
