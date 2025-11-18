#pragma once

struct Node {
	bool free;	// true if part of the list, otherwise false
	int next;	// index of next node in the linked list
};

class PoolAllocator
{
private:
	void *_address = nullptr;
	int _head = 0; // Index of the first free slot
	int _n = -1; // Number of blocks
	int _size = -1; // Size of a block
	Node *_nodes = nullptr; // Indices of the next used slot

	bool Expand();

public:
	PoolAllocator() = default;
	~PoolAllocator();
	bool Init(int n, int size);
	void *Request(); // Get the first free slot
	bool Free(void *element);

};

