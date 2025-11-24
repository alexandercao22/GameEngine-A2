#pragma once

struct Buddy {
	unsigned int size = 0;
	int state = 0; // 0 = Free, 1 - Used, 2 - Split
	void *ptr = nullptr;
};

class BuddyAllocator
{
private:
	int _size = 0;
	void *_memory = nullptr;
	const int _maxDepthSize = 32;

	Buddy *_buddies = nullptr;
	int _numBuddies = 0;

public:
	BuddyAllocator() = default;
	~BuddyAllocator();

	bool Init(unsigned int size = 1024);
	void *Request(unsigned int size);
	bool Free(void *element);

	// TODO: GetStats(); // Return BuddyStats struct

	// Returns address of the allocators memory
	void *GetAddress();
	// Prints the state of every possible buddy
	void PrintStates();
};

// Buddy Allocator:
// https://bitsquid.blogspot.com/2015/08/allocation-adventures-3-buddy-allocator.html