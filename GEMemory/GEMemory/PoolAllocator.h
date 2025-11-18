#pragma once

class PoolAllocator
{
private:
	void *_address = nullptr;
	int _head = 0; // Index of the first free slot
	int _n = -1; // Number of blocks
	int _size = -1; // Size of a block
	int *_nodes = nullptr; // Indices of the next used slot

	bool Expand();

public:
	PoolAllocator() = default;
	~PoolAllocator();
	bool Init(int n, int size);
	void *Request(); // Get the first free slot
	bool Free(void *element);

};

