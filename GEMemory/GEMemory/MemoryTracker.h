#pragma once

#include <string>
#include <unordered_map>

enum class Allocator {
	Pool,
	Buddy
};

struct Allocation {
	Allocator allocator;
	void* ptr;
	size_t size;	// Size in bytes
	std::string tag;	// Tag describing or categorizing the allocation
	// int creationTime;
};

class MemoryTracker 
{
private:
	// Keeps track of all tracked allocations using their pointers as keys for quick lookup
	std::unordered_map<void*, Allocation> _allocations;

public:
	// Records a new allocation
	void StartTracking(Allocator allocator, void* ptr, size_t size, std::string tag);
	// Removes an allocation from the record
	void StopTracking(void* ptr);

	// UI functionality
};

