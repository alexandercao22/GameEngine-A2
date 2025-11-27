#include <iostream>
#include "PoolAllocator.h"
#include "StackAllocator.h"
#include "BuddyAllocator.h"
#include "TestCases.h"

#include <iomanip>
#include <ctime>
#include <sstream>

//struct Enemy {
//	float health = 100.0f;
//	int legs = 3;
//	char tag = 'a';
//};

struct MainCharacter {
	float dmg;
};

// Time to string formatting
inline std::string FormatTimePoint(const std::chrono::system_clock::time_point& tp) {
	std::time_t timeStamp = std::chrono::system_clock::to_time_t(tp);
	std::tm localTime{};

	localtime_s(&localTime, &timeStamp); // thread-safe Windows version

	std::ostringstream oss;
	oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
	return oss.str();
}

void TestPool() {
	std::cout << "Testing PoolAllocator" << std::endl;

	PoolAllocator firstPool;
	firstPool.Init(5, 9);
	// PoolAllocator secondPool;
	// secondPool.Init(5, 32, true);

	std::cout << "Address to Pool 1: " << firstPool.GetAdress(0) << std::endl;

	Enemy *firstPtr = (Enemy *)firstPool.Request();
	firstPtr->health = 12.34f;
	firstPtr->legs = 27;
	firstPtr->tag = 'y';
	Allocation allocation;

	// Memory tracking testing

	std::unordered_map<int, PoolStats> stackAllocators = MemoryTracker::Instance().GetPoolAllocators();
	std::cout << "---- Tracked Pool Allocators ----" << std::endl;
	for (auto& pair : stackAllocators) {
		int allocatorId = pair.first;
		PoolStats allocator = pair.second;
		std::cout << std::endl << "Allocator id: " << allocatorId << std::endl;
		std::cout << "Capacity: " << allocator.capacity << std::endl;
		std::cout << "Used Memory: " << allocator.usedMemory << std::endl;
		std::cout << "Number Of Blocks: " << allocator.numBlocks << std::endl;
	}

	std::cout << std::endl;

	// Print all allocations on any stack
	std::unordered_map<void*, Allocation> allocations = MemoryTracker::Instance().GetAllocations();
	std::cout << "---- Tracked Pool Allocations ----" << std::endl;
	for (auto& pair : allocations) {
		Allocation allocation = pair.second;
		if (allocation.allocator == Allocator::Pool) {
			std::cout << std::endl;
			std::cout << "Allocator id: " << allocation.allocatorId << std::endl;
			std::cout << "Pointer: " << allocation.ptr << std::endl;
			std::cout << "Size: " << allocation.size << std::endl;
			std::cout << "Tag: " << allocation.tag << std::endl;
			std::cout << "Timestamp: " << FormatTimePoint(allocation.timestamp) << std::endl;
		}
	}

	//std::cout << "First value: " << firstPtr->health << std::endl;
	//std::cout << "First value: " << firstPtr->legs << std::endl;
	//std::cout << "First value: " << firstPtr->tag << std::endl;
	//std::cout << "F�rsta pointers adress: " << firstPtr << std::endl;

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

	//std::cout << "Fj�rde pointers adress: " << fourthPtr << std::endl;

	//void* fifthPtr = firstPool.Request();

	//std::cout << "Femte pointers adress: " << fifthPtr << std::endl;

	//firstPool.Free(secondPtr);

	//void* sixthPtr = firstPool.Request();

	//std::cout << "Sj�tte pointers adress: " << sixthPtr << std::endl;
	//

	//void* seventhPtr = firstPool.Request();
	//
	//std::cout << "Sjunde pointers adress: " << seventhPtr << std::endl;

	//std::cout << "SecondPool: " << secondPool.GetAdress(0) << std::endl;
	//std::cout << "SecondPool: " << secondPool.GetAdress() << std::endl;

	std::cout << std::endl;
}

void TestPoolTime() {
	int startObjects = 2000;
	std::cout << "Testing PoolAllocation " << std::endl;
	auto t0 = std::chrono::high_resolution_clock::now();

	PoolAllocator pool;
	pool.Init(startObjects, sizeof(Enemy));
	std::vector<Enemy*> live;

	const int FRAMES = 100'000;

	for (int f = 0; f < FRAMES; f++) {

		// Allocate some random objects
		int allocCount = rand() % 1000;
		for (int i = 0; i < allocCount; i++) {
			Enemy* e = (Enemy*)pool.Request();
			live.push_back(e);
		}

		// Free some random objects
		int freeCount = rand() % 1000;
		for (int i = 0; i < freeCount && !live.empty(); i++) {
			int index = rand() % live.size();
			pool.Free(live[index]);
			live.erase(live.begin() + index);
		}
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = t1 - t0;

	std::cout << "OS PoolAllocation Execution time: " << duration.count() << std::endl;
	
	std::cout << "Testing OS new/delete " << std::endl;
	t0 = std::chrono::high_resolution_clock::now();

	std::vector<Enemy*> live2;

	
	for (int f = 0; f < FRAMES; f++) {

		// Allocate some random objects
		int allocCount = rand() % 1000;
		for (int i = 0; i < allocCount; i++) {
			Enemy* e = new Enemy;
			live2.push_back(e);
		}

		// Free some random objects
		int freeCount = rand() % 1000;
		for (int i = 0; i < freeCount && !live2.empty(); i++) {
			int index = rand() % live2.size();
			delete live2[index];
			live2.erase(live2.begin() + index);
		}
	}
	t1 = std::chrono::high_resolution_clock::now();
	duration = t1 - t0;

	std::cout << "OS new/delete Execution time: " << duration.count() << std::endl;

	
	
	
	/* Running 2 loops, one uses new and delete each iteration the other uses poolallocation
	* Both loops are nestled which means for each outer iteration we can reuse the space from
	* PoolAllocation
	*/

	//auto t0 = std::chrono::high_resolution_clock::now();

	//std::vector<Enemy *> enemies;

	//for (int i = 0; i < startObjects; i++) {
	//	Enemy* enemy = new Enemy;
	//	enemies.push_back(enemy);
	//}

	//while (!enemies.empty()) {
	//	int insert = rand() % 10;

	//	if (insert % 10 == 0) {
	//		Enemy* enemy = new Enemy;
	//		enemies.push_back(enemy);
	//	}
	//	else {
	//		int max = enemies.size();
	//		int index = rand() % max;
	//		
	//		delete enemies[index];
	//		enemies.erase(enemies.begin() + index);
	//	}

	//	/*if (enemies.size() % 100 == 0) {
	//		std::cout << "iterating" << std::endl;

	//	}*/

	//}

	//auto t1 = std::chrono::high_resolution_clock::now();

	//std::chrono::duration<double> duration = t1 - t0;
	//std::cout << "OS new/delete Execution time: " << duration.count() << std::endl;

	////for (int i = 0; i < nrOfObjects; i++) {
	////	for (int k = 0; k < 10; k++) {
	////		Enemy *enemy = new Enemy;
	////		enemies.push_back(enemy);
	////	}

	////	for (auto enemy : enemies) {
	////		delete enemy;
	////	}
	////	enemies.clear();
	////}
	//t0 = std::chrono::high_resolution_clock::now();

	//PoolAllocator PoolAlloc;
	//PoolAlloc.Init(startObjects, sizeof(Enemy));

	//std::vector<Enemy *> enemies2;


	//for (int i = 0; i < startObjects; i++) {
	//		Enemy *enemy = (Enemy *)PoolAlloc.Request();
	//		enemies2.push_back(enemy);

	//}
	//while (!enemies2.empty()) {
	//	int insert = rand() % 10;

	//	if (insert % 10 == 0) {
	//		Enemy* enemy = (Enemy*)PoolAlloc.Request();
	//		enemies2.push_back(enemy);
	//	}
	//	else {
	//		int max = enemies2.size();
	//		int index = rand() % max;

	//		
	//		PoolAlloc.Free(enemies2.at(index));

	//		enemies2.erase(enemies2.begin() + index);
	//	}

	//	/*if (enemies.size() % 100 == 0) {
	//		std::cout << "iterating" << std::endl;

	//	}*/

	//}

	//


	//t1 = std::chrono::high_resolution_clock::now();

	//duration = t1 - t0;
	//std::cout << "OS PoolAllocator Execution time: " << duration.count() << std::endl;
}

void TestBuddy() {
	std::cout << "Testing BuddyAllocator" << std::endl;

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


	// Memory tracking testing

	std::unordered_map<int, BuddyStats> buddyAllocators = MemoryTracker::Instance().GetBuddyAllocators();
	std::cout << "---- Tracked Buddy Allocators ----" << std::endl;
	for (auto& pair : buddyAllocators) {
		int allocatorId = pair.first;
		BuddyStats allocator = pair.second;
		std::cout << std::endl << "Allocator id: " << allocatorId << std::endl;
		std::cout << "Capacity: " << allocator.capacity << std::endl;
		std::cout << "Used Memory: " << allocator.usedMemory << std::endl;
	}

	std::cout << std::endl;

	// Print all allocations in in any buddy allocator
	std::unordered_map<void*, Allocation> allocations = MemoryTracker::Instance().GetAllocations();
	std::cout << "---- Tracked Buddy Allocations ----" << std::endl;
	for (auto& pair : allocations) {
		Allocation allocation = pair.second;
		if (allocation.allocator == Allocator::Buddy) {
			std::cout << std::endl;
			std::cout << "Allocator id: " << allocation.allocatorId << std::endl;
			std::cout << "Pointer: " << allocation.ptr << std::endl;
			std::cout << "Size: " << allocation.size << std::endl;
			std::cout << "Tag: " << allocation.tag << std::endl;
			std::cout << "Timestamp: " << FormatTimePoint(allocation.timestamp) << std::endl;
		}
	}

	for (int i = 0; i < 16; i++) {
		if (i % 2 == 0) {
			buddyAllocator.Free(ptrs[i]);
		}
	}
	buddyAllocator.PrintStates();

	std::cout << std::endl;
}

void TestStack() {

	std::cout << "Testing OS Allocator" << std::endl;
	// testing variables
	int nrOfIterations = 10000;
	int nrOfObjects = 100;

	// Testing allocations framewise with "fixed size" enemies
	auto t0 = std::chrono::high_resolution_clock::now();
	std::vector<Enemy*> enemies;

	for (int i = 0; i < nrOfIterations; i++) {
		for (int k = 0; k < nrOfObjects; k++) {
			Enemy* enemy = new Enemy;
			enemies.push_back(enemy);
		}

		for (auto enemy : enemies) {
			delete enemy;
		}
		enemies.clear();
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> duration = t1 - t0;
	std::cout << "OS Allocator exection time" << duration.count() << std::endl;

	
	std::cout << "Testing StackAllocator: " << std::endl;
	// Testing allocation framewise with our Stack
	// reseting each iteration instead of deleting them
	StackAllocator firstStack;
	firstStack.Initialize(100000);
	t0 = std::chrono::high_resolution_clock::now();
	
	for (int i = 0; i < nrOfIterations; i++) {
		for (int k = 0; k < nrOfObjects; k++) {
			/*int size = rand() % 100;*/
			void* ptr = firstStack.Request(sizeof(Enemy));
		}
		firstStack.Reset();
		
	}
	t1 = std::chrono::high_resolution_clock::now();
	duration = t1 - t0;
		
	std::cout << "StackAllocator exection time: " << duration.count() << std::endl;


	std::cout << "Testing OS Stack " << std::endl;
	// Testing variables in the OS Stack
	t0 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < nrOfIterations; i++) {
		for (int k = 0; k < nrOfObjects; k++) {
			Enemy enemy;
			enemy.health = 100;
			enemy.legs = 2;
			enemy.tag = 'a';
		}

	/*	if (i % 100 == 0)
			std::cout << "iteraions" << std::endl;
	*/
	}
	t1 = std::chrono::high_resolution_clock::now();

	duration = t1 - t0;

	std::cout << "OS Stack exection time: " << duration.count() << std::endl;

	//StackAllocator secondStack;
	//secondStack.Initialize(600);

	//Enemy *firstPtr = (Enemy *)firstStack.Request(sizeof(Enemy));
	//std::cout << "firstPtr address: " << firstPtr << std::endl;

	//firstPtr->health = 69.67f;
	//firstPtr->legs = 2;
	//firstPtr->tag = 'b';

	//std::cout << "First value: " << firstPtr->health << std::endl;
	//std::cout << "First value: " << firstPtr->legs << std::endl;
	//std::cout << "First value: " << firstPtr->tag << std::endl;
	//std::cout << "firstPtr address: " << firstPtr << std::endl;

	//Enemy *secondPtr = (Enemy *)firstStack.Request(sizeof(Enemy));
	//secondPtr->health = 13.37f;
	//secondPtr->legs = 2;
	//secondPtr->tag = 'c';

	//std::cout << "Second value: " << secondPtr->health << std::endl;
	//std::cout << "Second value: " << secondPtr->legs << std::endl;
	//std::cout << "Second value: " << secondPtr->tag << std::endl;
	//std::cout << "secondPtr address: " << secondPtr << std::endl;

	//firstStack.Free();

	//MainCharacter *thirdPtr = (MainCharacter *)firstStack.Request(sizeof(MainCharacter));
	//thirdPtr->dmg = 14.86;

	//std::cout << "Third value: " << thirdPtr->dmg << std::endl;
	//std::cout << "thirdPtr address: " << thirdPtr << std::endl;


	//MainCharacter *fourthPtr = (MainCharacter *)firstStack.Request(sizeof(MainCharacter));
	//fourthPtr->dmg = 19.19;

	//std::cout << "Fourth value: " << fourthPtr->dmg << std::endl;
	//std::cout << "fourthPtr address: " << fourthPtr << std::endl;

	std::cout << std::endl;

	// Memory tracking testing

	//MemoryTracker::Instance().TrackAllocator(firstStack.GetId(), firstStack.GetStats());
	//StackStats stats;
	//MemoryTracker::Instance().GetAllocatorStats(0, stats);
	//std::cout << std::endl << "---- StackAllocator [id=0] ----" << std::endl;
	//std::cout << "Capacity: " << stats.capacity << std::endl;
	//std::cout << "Used Memory: " << stats.usedMemory << std::endl;

	std::unordered_map<int, StackStats> stackAllocators = MemoryTracker::Instance().GetStackAllocators();
	std::cout << "---- Tracked Stack Allocators ----" << std::endl;
	for (auto& pair : stackAllocators) {
		int allocatorId = pair.first;
		StackStats allocator = pair.second;
		std::cout << std::endl << "Allocator id: " << allocatorId << std::endl;
		std::cout << "Capacity: " << allocator.capacity << std::endl;
		std::cout << "Used Memory: " << allocator.usedMemory << std::endl;
	}

	std::cout << std::endl;

	// Print all allocations on any stack
	std::unordered_map<void*, Allocation> allocations = MemoryTracker::Instance().GetAllocations();
	std::cout << "---- Tracked Stack Allocations ----" << std::endl;
	for (auto& pair : allocations) {
		Allocation allocation = pair.second;
		if (allocation.allocator == Allocator::Stack) {
			std::cout << std::endl;
			std::cout << "Allocator id: " << allocation.allocatorId << std::endl;
			std::cout << "Pointer: " << allocation.ptr << std::endl;
			std::cout << "Size: " << allocation.size << std::endl;
			std::cout << "Tag: " << allocation.tag << std::endl;
			std::cout << "Timestamp: " << FormatTimePoint(allocation.timestamp) << std::endl;
		}
	}

}

int main() {
	//TestPool();
	//TestBuddy();
	TestStack();

	//TestPoolTime();

	PoolVSOS();

	return 0;
}