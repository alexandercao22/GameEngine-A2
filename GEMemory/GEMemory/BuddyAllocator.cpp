#include "BuddyAllocator.h"

#include <iostream>
#include <cmath>
#include <malloc.h>

BuddyAllocator::~BuddyAllocator()
{
	free(_memory);
	free(_buddies);
}

bool BuddyAllocator::Init(unsigned int size)
{
	// Check if size is of base 2
	float log2Size = std::log(size) / std::log(2);
	if (ceilf(log2Size) != log2Size) {
		std::cout << "BuddyAllocator::Init(): Failed to initalize" << std::endl;
		return false;
	}

	_memory = malloc(size);
	if (!_memory) {
		std::cerr << "BuddyAllocator::Init(): Failed to allocate memory" << std::endl;
		return false;
	}

	// Calculate how many slots are needed
	for (int i = 0; i < log2Size - (std::log(_maxDepthSize) / std::log(2)) + 1; i++) {
		_numBuddies += std::pow(2, i);
	}
	_buddies = (Buddy *)malloc(static_cast<size_t>(sizeof(Buddy) * _numBuddies));
	if (!_buddies) {
		std::cerr << "BuddyAllocator::Init(): Failed to allocate buddies" << std::endl;
		return false;
	}

	int level = 0;
	int buddiesToInit = std::pow(2, level); // Amount of buddies to initialize on current level
	int buddiesInit = 0; // Amount of buddies initialized on current level
	for (int i = 0; i < _numBuddies; i++) {
		_buddies[i].size = size / std::pow(2, level);
		_buddies[i].state = 0;
		_buddies[i].ptr = (char *)_memory + (_buddies[i].size * buddiesInit);

		buddiesInit++;
		if (buddiesInit == buddiesToInit) {
			level++;
			buddiesToInit = std::pow(2, level);
			buddiesInit = 0;
		}
	}

	return true;
}

void *BuddyAllocator::Request(unsigned int size)
{

}

bool BuddyAllocator::Free(void *element)
{
	return false;
}

void *BuddyAllocator::GetAddress()
{

}

