#pragma once
#include <vector>

struct Node {
	bool free;	// True if part of the list, otherwise false
	int next;	// Index of next node in the linked list
};

struct Block {
	void* address = nullptr;
	Node* nodes = nullptr;	// Pool slots tracker
	int head ;	// Index of the first free slot (-1 means no empty slots)
};

class PoolAllocator
{
private:
	std::vector<Block> _blocks;

	int _n = -1;	// Number of slots contained in a single block (-1 = uninitialized)
	int _size = -1;	// Size of the slots in the pool (-1 = uninitialized)

	bool _aligned = false;

	bool InitBlock(Block *block);
	bool Expand();

public:
	PoolAllocator() = default;
	~PoolAllocator();
	bool Init(int n, int size, bool aligned = false); // If aligned is true, initial memory is set on size % = 0
	void *Request(); // Get the first free slot
	bool Free(void *element);
	void* GetAdress();

};

