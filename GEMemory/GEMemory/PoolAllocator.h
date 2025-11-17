#pragma once

class PoolAllocator
{
private:
	void *_adress;
	int _head; // Index of the first free slot
	int _n; // Number of blocks
	int _size; // Size of a block
	int *_nodes; // Index of the next used slot

	bool Expand();

public:
	PoolAllocator() = default;
	~PoolAllocator();
	bool Init(int n, int size);
	void *Request(); // Get the first free slot
	bool Free(void *element);

};

