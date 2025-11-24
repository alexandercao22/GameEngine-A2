#include <iostream>
#include "PoolAllocator.h"
#include "StackAllocator.hpp"

struct Enemy
{
	float health = 100.0f;
	int legs = 3;
	char tag = 'a';
};

struct MainCharacter {
	float dmg;
};
int main() {
	std::cout << "Hello World!" << std::endl;

	//PoolAllocator firstPool;
	//firstPool.Init(5, 9);
	//PoolAllocator secondPool;
	//secondPool.Init(5, 32, true);

	//std::cout << "Första pointers adress: " << firstPool.GetAdress(0) << std::endl;

	//Enemy* firstPtr = (Enemy *)firstPool.Request("First enemy");
	//firstPtr->health = 69.67f;
	//firstPtr->legs = 2;
	//firstPtr->tag = 'b';

	//Allocation allocation;
	//MemoryTracker::Instance().GetAllocation(firstPtr, &allocation);
	//std::cout << "Allocation tag: " << allocation.tag << std::endl;

	//firstPool.Free(firstPtr);
	StackAllocator firstStack;
	firstStack.Initialize(24);

	Enemy* firstPtr2 = (Enemy*)firstStack.Request(sizeof(Enemy));
	std::cout << "Första pointers adress: " << firstPtr2 << std::endl;

	
	firstPtr2->health = 69.67f;
	firstPtr2->legs = 2;
	firstPtr2->tag = 'b';
	
	std::cout << "First value: " << firstPtr2->health << std::endl;
	std::cout << "First value: " << firstPtr2->legs << std::endl;
	std::cout << "First value: " << firstPtr2->tag << std::endl;
	std::cout << "Första pointers adress: " << firstPtr2 << std::endl;
	
	Enemy* secondPtr = (Enemy*)firstStack.Request(sizeof(Enemy));
	std::cout << "Första pointers adress: " << firstPtr2 << std::endl;


	secondPtr->health = 13.37f;
	secondPtr->legs = 2;
	secondPtr->tag = 'c';

	std::cout << "Second value: " << secondPtr->health << std::endl;
	std::cout << "Second value: " << secondPtr->legs << std::endl;
	std::cout << "Second value: " << secondPtr->tag << std::endl;
	std::cout << "Second pointers adress: " << secondPtr << std::endl;

	firstStack.Free();


	MainCharacter* thirdPtr = (MainCharacter*)firstStack.Request(sizeof(MainCharacter));
	thirdPtr->dmg = 14.86;

	std::cout << "Third value: " << thirdPtr->dmg << std::endl;
	std::cout << "third pointers adress: " << thirdPtr << std::endl;


	MainCharacter* fourthPtr = (MainCharacter*)firstStack.Request(sizeof(MainCharacter));
	fourthPtr->dmg = 19.19;

	std::cout << "Fourth value: " << fourthPtr->dmg << std::endl;
	std::cout << "fourth pointers adress: " << fourthPtr << std::endl;

	/*
	Enemy *secondPtr = (Enemy *)firstPool.Request();
	firstPtr->health = 12.34f;
	firstPtr->legs = 27;
	firstPtr->tag = 'y';


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