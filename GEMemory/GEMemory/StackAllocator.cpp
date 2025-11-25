#include "StackAllocator.h"

bool StackAllocator::Initialize(int size) {

	_size = size;
	_start = malloc(size);
	_head = _start;
	int N = size / 4;
	_blockSize = new int[N];
	if (!_head) {
		std::cerr << "StackAllocator failed to allocate block" << std::endl;
		return false;
	}
	return true;
}

StackAllocator::~StackAllocator() {
	free(_start);
	delete _blockSize;
}

// Copy a pointer to the start of the block and update head
void* StackAllocator::Request(int size) {

	void* block = _head;

	if (static_cast<char*>(_head) + size > static_cast<char*>(_start) + _size) {
		std::cerr << "Failed to allocate on the Stack: Not enough space available" << std::endl;
		return nullptr;
	}
	_head = static_cast<char*>(_head) + size;


	_index++;
	_blockSize[_index] = size;

	return block;
}

bool StackAllocator::Free() {

	_head = static_cast<char*>(_head) - _blockSize[_index];
	_index--;

	return true;
}

bool StackAllocator::Reset() {
	_head = _start;
	_index = -1;

	return true;
}