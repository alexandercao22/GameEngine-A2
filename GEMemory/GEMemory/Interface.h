#pragma once

#include <vector>

#include "PoolAllocator.h"
#include "BuddyAllocator.h"
#include "StackAllocator.h"

struct PoolContainer {
	PoolAllocator *pool;
	std::vector<void *> ptrs;
};

struct BuddyContainer {
	BuddyAllocator *buddy;
	std::vector<void *> ptrs;
};

struct StackContainer {
	StackAllocator *stack;
	std::vector<void *> ptrs;
};

class Interface
{
private:
	const int _width = 700;
	const int _height = 700;

	std::vector<PoolContainer> _pools;
	std::vector<BuddyContainer> _buddies;
	std::vector<StackContainer> _stacks;

	void ShowPoolInfo();
	void ShowBuddyInfo();
	void ShowStackInfo();

public:
	Interface();
	~Interface();

	bool Init();

	void Update();
};

