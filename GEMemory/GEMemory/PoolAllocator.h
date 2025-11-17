#pragma once

struct Node
{
	int index;
	int child;
};

class PoolAllocator
{
private:
	void *adress;
	int head; // Index of the first free slot
	int n; // Number of blocks
	int size; // Size of a block
	int *nodes;

	bool Expand();

public:
	PoolAllocator() = default;
	~PoolAllocator();
	bool Init(int n, int size);
	void *Request(); // Get the first free slot
	bool Free(void *element);

};

