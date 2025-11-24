#include <iostream>
#include "PoolAllocator.h"
#include "BuddyAllocator.h"

struct Enemy
{
	float health = 100.0f;
	int legs = 3;
	char tag = 'a';
};

int main() {
	std::cout << "Hello World!" << std::endl;

	//PoolAllocator firstPool;
	//firstPool.Init(5, 9);
	//PoolAllocator secondPool;
	//secondPool.Init(5, 32, true);

	//std::cout << "Första pointers adress: " << firstPool.GetAdress() << std::endl;

	//Enemy* firstPtr = (Enemy *)firstPool.Request();
	//firstPtr->health = 69.67f;
	//firstPtr->legs = 2;
	//firstPtr->tag = 'b';

	//std::cout << "First value: " << firstPtr->health << std::endl;
	//std::cout << "First value: " << firstPtr->legs << std::endl;
	//std::cout << "First value: " << firstPtr->tag << std::endl;
	//std::cout << "Första pointers adress: " << firstPtr << std::endl;

	//Enemy *secondPtr = (Enemy *)firstPool.Request();
	//firstPtr->health = 12.34f;
	//firstPtr->legs = 27;
	//firstPtr->tag = 'y';

	//std::cout << "Second value: " << firstPtr->health << std::endl;
	//std::cout << "Second value: " << firstPtr->legs << std::endl;
	//std::cout << "Second value: " << firstPtr->tag << std::endl;
	//std::cout << "Andra pointers adress: " << secondPtr << std::endl;

	//void* thirdPtr = firstPool.Request();

	//std::cout << "Tredje pointers adress: " << thirdPtr << std::endl;

	//void* fourthPtr = firstPool.Request();

	//std::cout << "Fjärde pointers adress: " << fourthPtr << std::endl;

	//void* fifthPtr = firstPool.Request();

	//std::cout << "Femte pointers adress: " << fifthPtr << std::endl;

	//firstPool.Free(secondPtr);

	//void* sixthPtr = firstPool.Request();

	//std::cout << "Sjätte pointers adress: " << sixthPtr << std::endl;
	//

	//void* seventhPtr = firstPool.Request();
	//
	//std::cout << "Sjunde pointers adress: " << seventhPtr << std::endl;

	//std::cout << "SecondPool: " << secondPool.GetAdress() << std::endl;

	BuddyAllocator buddyAllocator;
	buddyAllocator.Init(512);
	//void *ptr1 = buddyAllocator.Request(32);
	//buddyAllocator.PrintStates();
	//void *ptr2 = buddyAllocator.Request(64);
	//buddyAllocator.PrintStates();
	//void *ptr3 = buddyAllocator.Request(30);
	//buddyAllocator.PrintStates();

	//buddyAllocator.Free(ptr3);
	//buddyAllocator.PrintStates();
	//buddyAllocator.Free(ptr1);
	//buddyAllocator.PrintStates();

	//ptr3 = buddyAllocator.Request(30);
	//buddyAllocator.PrintStates();
	//ptr1 = buddyAllocator.Request(129);
	//buddyAllocator.PrintStates();

	std::vector<void *> ptrs;
	for (int i = 0; i < 16; i++) {
		void *ptr = buddyAllocator.Request(32);
		ptrs.push_back(ptr);
	}
	buddyAllocator.PrintStates();

	for (int i = 0; i < 16; i++) {
		if (i % 2 == 0) {
			buddyAllocator.Free(ptrs[i]);
		}
	}
	buddyAllocator.PrintStates();

	return 0;
}