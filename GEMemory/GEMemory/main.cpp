#include <iostream>
#include "PoolAllocator.h"
#include "StackAllocator.hpp"

struct Enemy
{
	float health = 100.0f;
	int legs = 3;
	char tag = 'a';
};

int main() {
	std::cout << "Hello World!" << std::endl;

	PoolAllocator firstPool;
	firstPool.Init(5, 9);
	PoolAllocator secondPool;
	secondPool.Init(5, 32, true);

	std::cout << "Första pointers adress: " << firstPool.GetAdress(0) << std::endl;

	Enemy* firstPtr = (Enemy *)firstPool.Request("First enemy");
	firstPtr->health = 69.67f;
	firstPtr->legs = 2;
	firstPtr->tag = 'b';

	Allocation allocation;
	MemoryTracker::Instance().GetAllocation(firstPtr, &allocation);
	std::cout << "Allocation tag: " << allocation.tag << std::endl;

	firstPool.Free(firstPtr);
	StackAllocator firstStack;
	firstStack.Initialize(12);

	Enemy* firstPtr = (Enemy*)firstStack.Request(sizeof(Enemy));
	std::cout << "Första pointers adress: " << firstPtr << std::endl;

	/*std::cout << "Första pointers adress: " << firstPool.GetAdress() << std::endl;
	*/
	//Enemy* firstPtr = (Enemy *)firstPool.Request();
	firstPtr->health = 69.67f;
	firstPtr->legs = 2;
	firstPtr->tag = 'b';
	
	std::cout << "First value: " << firstPtr->health << std::endl;
	std::cout << "First value: " << firstPtr->legs << std::endl;
	std::cout << "First value: " << firstPtr->tag << std::endl;
	std::cout << "Första pointers adress: " << firstPtr << std::endl;
	/*
	Enemy *secondPtr = (Enemy *)firstPool.Request();
	firstPtr->health = 12.34f;
	firstPtr->legs = 27;
	firstPtr->tag = 'y';
>>>>>>> Stashed changes

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

<<<<<<< Updated upstream
	//std::cout << "Sjätte pointers adress: " << sixthPtr << std::endl;
	//

	//void* seventhPtr = firstPool.Request();
	//
	//std::cout << "Sjunde pointers adress: " << seventhPtr << std::endl;

	//std::cout << "SecondPool: " << secondPool.GetAdress(0) << std::endl;
=======
	std::cout << "SecondPool: " << secondPool.GetAdress() << std::endl;*/

	return 0;
}