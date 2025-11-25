#include <iostream>
#include "PoolAllocator.h"
#include "StackAllocator.h"
#include "BuddyAllocator.h"

#include <iomanip>
#include <ctime>
#include <sstream>

struct Enemy {
	float health = 100.0f;
	int legs = 3;
	char tag = 'a';
};

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
	int nrOfObjects = 50000;

	/* Running 2 loops, one uses new and delete each iteration the other uses poolallocation
	* Both loops are nestled which means for each outer iteration we can reuse the space from
	* PoolAllocation
	*/

	auto t0 = std::chrono::high_resolution_clock::now();

	std::vector<Enemy *> enemies;


	for (int i = 0; i < nrOfObjects; i++) {
		for (int k = 0; k < 10; k++) {
			Enemy *enemy = new Enemy;
			enemies.push_back(enemy);
		}

		for (auto enemy : enemies) {
			delete enemy;
		}
		enemies.clear();
	}
	auto t1 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> duration = t1 - t0;
	std::cout << "OS new/delete Execution time: " << duration.count() << std::endl;

	t0 = std::chrono::high_resolution_clock::now();

	PoolAllocator PoolAlloc;
	PoolAlloc.Init(20, sizeof(Enemy));

	std::vector<Enemy *> enemies2;


	for (int i = 0; i < nrOfObjects; i++) {
		for (int k = 0; k < 10; k++) {
			Enemy *enemy = (Enemy *)PoolAlloc.Request();
			enemies2.push_back(enemy);

		}
		for (auto enemy : enemies2) {
			PoolAlloc.Free(enemy);
		}
		enemies2.clear();
	}


	t1 = std::chrono::high_resolution_clock::now();

	duration = t1 - t0;
	std::cout << "OS PoolAllocator Execution time: " << duration.count() << std::endl;
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
	std::cout << "Testing StackAllocator" << std::endl;

	StackAllocator firstStack;
	firstStack.Initialize(24);

	StackAllocator secondStack;
	secondStack.Initialize(600);

	Enemy *firstPtr = (Enemy *)firstStack.Request(sizeof(Enemy));
	std::cout << "firstPtr address: " << firstPtr << std::endl;

	firstPtr->health = 69.67f;
	firstPtr->legs = 2;
	firstPtr->tag = 'b';

	std::cout << "First value: " << firstPtr->health << std::endl;
	std::cout << "First value: " << firstPtr->legs << std::endl;
	std::cout << "First value: " << firstPtr->tag << std::endl;
	std::cout << "firstPtr address: " << firstPtr << std::endl;

	Enemy *secondPtr = (Enemy *)firstStack.Request(sizeof(Enemy));
	secondPtr->health = 13.37f;
	secondPtr->legs = 2;
	secondPtr->tag = 'c';

	std::cout << "Second value: " << secondPtr->health << std::endl;
	std::cout << "Second value: " << secondPtr->legs << std::endl;
	std::cout << "Second value: " << secondPtr->tag << std::endl;
	std::cout << "secondPtr address: " << secondPtr << std::endl;

	firstStack.Free();

	MainCharacter *thirdPtr = (MainCharacter *)firstStack.Request(sizeof(MainCharacter));
	thirdPtr->dmg = 14.86;

	std::cout << "Third value: " << thirdPtr->dmg << std::endl;
	std::cout << "thirdPtr address: " << thirdPtr << std::endl;


	MainCharacter *fourthPtr = (MainCharacter *)firstStack.Request(sizeof(MainCharacter));
	fourthPtr->dmg = 19.19;

	std::cout << "Fourth value: " << fourthPtr->dmg << std::endl;
	std::cout << "fourthPtr address: " << fourthPtr << std::endl;

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
	TestPool();
	TestBuddy();
	TestStack();

	//TestPoolTime();

	return 0;
}