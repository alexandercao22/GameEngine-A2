#include "MemoryTracker.h"
#include <iostream>

void MemoryTracker::StartTracking(Allocator allocator, void* ptr, size_t size, std::string tag)
{
	Allocation allocation;
	allocation.allocator = allocator;
	allocation.ptr = ptr;
	allocation.size = size;
	allocation.tag = tag;
	allocation.timestamp = std::chrono::high_resolution_clock::now();

	_allocations.emplace(ptr, allocation);
}

void MemoryTracker::StopTracking(void* ptr)
{
	_allocations.erase(ptr);
}

bool MemoryTracker::GetAllocation(void* ptr, Allocation* allocation)
{
	auto element = _allocations.find(ptr);
	if (element == _allocations.end()) {
		std::cerr << "MemoryTracker::GetAllocation(): pointer is not being tracked" << std::endl;
		return false;
	}

	*allocation = element->second; // returns the item (allocation)
	return true;
}
