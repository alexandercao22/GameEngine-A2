#pragma once
#include <malloc.h>
#include <iostream>

class StackAllocator {
	
private:
	void* _start;
	void* _head;
	int* _blockSize;
	int _size;
	int _index = -1;

public:
	StackAllocator() = default;
	~StackAllocator();

	// Allocate memory space for the stack (bytes)
	bool Initialize(int size);

	void* Request(int size);
	bool Free();
	bool Reset();
};