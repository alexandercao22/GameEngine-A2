#pragma once

#include <string>
#include <unordered_map>
#include <chrono>

enum class Allocator {
	Pool,
	Buddy
};

struct Allocation {
	Allocator allocator;
	void* ptr;
	size_t size;	// Size in bytes
	std::string tag;	// Tag describing or categorizing the allocation
	std::chrono::high_resolution_clock::time_point timestamp; // Creation timestamp
};

class MemoryTracker 
{
private:
	MemoryTracker() = default;
	~MemoryTracker() = default;

	// Keeps track of all tracked allocations using their pointers as keys for quick lookup
	std::unordered_map<void*, Allocation> _allocations;

public:
	// Singleton instance
	static MemoryTracker& Instance() {
		static MemoryTracker instance;
		return instance;
	}
	// Copy prevention
	MemoryTracker(const MemoryTracker&) = delete;
	MemoryTracker& operator=(const MemoryTracker&) = delete;

	// Records a new allocation
	void StartTracking(Allocator allocator, void* ptr, size_t size, std::string tag);
	// Removes an allocation from the record
	void StopTracking(void* ptr);

	// UI functionality

	// Debug functionality
	bool GetAllocation(void* ptr, Allocation* allocation);
};

