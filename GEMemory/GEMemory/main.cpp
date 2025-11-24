#include <iostream>
#include "PoolAllocator.h"
#include "BuddyAllocator.h"

#include <iomanip>
#include <ctime>
#include <sstream>

struct Enemy
{
	float health = 100.0f;
	int legs = 3;
	char tag = 'a';
};

// Time to string formatting
inline std::string FormatTimePoint(const std::chrono::system_clock::time_point& tp)
{
	std::time_t timeStamp = std::chrono::system_clock::to_time_t(tp);
	std::tm localTime{};

	localtime_s(&localTime, &timeStamp); // thread-safe Windows version

	std::ostringstream oss;
	oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
	return oss.str();
}


int main() {
	std::cout << "Hello World!" << std::endl;

	PoolAllocator firstPool;
	firstPool.Init(5, 9);
	// PoolAllocator secondPool;
	// secondPool.Init(5, 32, true);

	std::cout << "Address to Pool 1: " << firstPool.GetAdress(0) << std::endl;

	Enemy* firstPtr = (Enemy *)firstPool.Request("First enemy");
	firstPtr->health = 69.67f;
	firstPtr->legs = 2;
	firstPtr->tag = 'b';

	Allocation allocation;
	MemoryTracker::Instance().GetAllocation(firstPtr, allocation);
	std::cout << "---- Allocation 1 ----" << std::endl;
	//std::cout << "Allocator type: " << allocation.allocator << std::endl;
	std::cout << std::endl << "Allocator id: " << allocation.allocatorId << std::endl;
	std::cout << "Pointer: " << allocation.ptr << std::endl;
	std::cout << "Size: " << allocation.size << std::endl;
	std::cout << "Tag: " << allocation.tag << std::endl;
	std::cout << "Timestamp: " << FormatTimePoint(allocation.timestamp) << std::endl;

	MemoryTracker::Instance().TrackAllocator(firstPool.GetId(), firstPool.GetStats());
	PoolStats poolStats;
	MemoryTracker::Instance().GetAllocatorStats(0, poolStats);
	std::cout << std::endl << "---- Pool Allocator [id=0] ----" << std::endl;
	std::cout << "Capacity: " << poolStats.capacity << std::endl;
	std::cout << "Used Memory: " << poolStats.usedMemory << std::endl;
	std::cout << "Number of Blocks: " << poolStats.numBlocks << std::endl;

	firstPool.Free(firstPtr);

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