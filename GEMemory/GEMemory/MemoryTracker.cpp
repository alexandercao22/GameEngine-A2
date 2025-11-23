#include "MemoryTracker.h"

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
