#pragma once
#include <malloc.h>
#include <iostream>

class StackAllocator {
	
private:
	void* _start;
	void* _head;
	int _size;
	int _blockSize[16];
	int _index = -1;

public:
	StackAllocator() = default;
	~StackAllocator();

	// Allocate memory space for the stack (bytes)
	bool Initialize(int size);

	void* Request(int size);
	bool Free();
};