#include "StackAllocator.h"

bool StackAllocator::Initialize(int size) {

	_size = size;
	_start = malloc(size);
	_head = _start;
	if (!_head) {
		std::cerr << "StackAllocator failed to allocate block" << std::endl;
		return false;
	}
	std::cout << "head adress: " << _head << std::endl;
	return true;
}

StackAllocator::~StackAllocator() {
	std::cout << "head adress: " << _head << std::endl;
	free(_start);
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

	int diff = static_cast<char*>(_head) - static_cast<char*>(_start);
	std::cout << "diff: " << diff << std::endl;
	return block;
}

bool StackAllocator::Free() {
	int diff = _blockSize[_index];
	std::cout << "free diff: " << diff << std::endl;
	_head = static_cast<char*>(_head) - _blockSize[_index];
	_index--;

	return true;
}