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
		if (size > _buddies[0].size) {
		std::cerr << "BuddyAllocator::Request(): The requested amount is too large" << std::endl;
		return nullptr;
	}

	if (_buddies[0].state == 1) {
		std::cerr << "BuddyAllocator::Request(): There's no free memory left" << std::endl;
		return nullptr;
	}

	int parentIdx = 0;
	for (int i = 0; i < _numBuddies; i++) {
		Buddy *current = &_buddies[i];
		if (size <= current->size / 2) { // Traverse to a fitting buddy size
			if (current->state == 0) {
				current->state = 2;
				parentIdx = i;
				i = i * 2; // Skip to the next level
				continue;
			}
			else if (current->state == 2) {
				i = i * 2; // Skip to the next level
				continue;
			}
			else {
				continue;
			}
		}
		else { // Found a fitting buddy size
			if (current->state == 0) {
				std::cout << "Buddy index: " << i << std::endl;
				current->state = 1;
				return current->ptr;
			}
			else if (current->state == 2) { // Current is split, move on to its buddy
				continue;
			}
			else if (i == parentIdx + 1) {
				return nullptr;
			}
		}
	}

	return nullptr;
}

bool BuddyAllocator::Free(void *element)
{
	return false;
}

void *BuddyAllocator::GetAddress()
{
	if (!_memory) {
		std::cerr << "BuddyAllocator::GetAddress(): Buddy allocator is uninitialized" << std::endl;
		return nullptr;
	}

	return _memory;
}

void BuddyAllocator::PrintStates()
{
	for (int i = 0; i < _numBuddies; i++) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < _numBuddies; i++) {
		if (i >= 10) {
			std::cout << " ";
		}
		std::cout << _buddies[i].state << " ";
	}
	std::cout << std::endl;
}
