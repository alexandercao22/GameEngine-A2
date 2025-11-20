#include <iostream>
#include "PoolAllocator.h"


int main() {
	std::cout << "Hello World!" << std::endl;

	PoolAllocator firstPool;
	firstPool.Init(5, 8);


	std::cout << "Första pointers adress: " << firstPool.GetAdress() << std::endl;

	void* firstPtr = firstPool.Request();

	std::cout << "Första pointers adress: " << firstPtr << std::endl;

	void* secondPtr = firstPool.Request();

	std::cout << "Andra pointers adress: " << secondPtr << std::endl;

	void* thirdPtr = firstPool.Request();

	std::cout << "Tredje pointers adress: " << thirdPtr << std::endl;

	void* fourthPtr = firstPool.Request();

	std::cout << "Fjärde pointers adress: " << fourthPtr << std::endl;

	void* fifthPtr = firstPool.Request();

	std::cout << "Femte pointers adress: " << fifthPtr << std::endl;

	firstPool.Free(secondPtr);

	void* sixthPtr = firstPool.Request();

	std::cout << "Sjätte pointers adress: " << sixthPtr << std::endl;
	

	void* seventhPtr = firstPool.Request();
	
	std::cout << "Sjunde pointers adress: " << seventhPtr << std::endl;


	return 0;
}