#include "PoolAllocator.h"

#include <malloc.h>
#include <iostream>

bool PoolAllocator::Expand()
{

	return false;
}

PoolAllocator::~PoolAllocator()
{
	free(_address);
	_address = nullptr;

	free(_nodes);
	_address = nullptr;
}

bool PoolAllocator::Init(int n, int size)
{
	_n = n;
	_size = size;

	_address = malloc(static_cast<size_t>(n * size));
	if (!_address) {
		std::cerr << "PoolAllocator::Init(): failed to allocate pool" << std::endl;
		return false;
	}

	_nodes = (Node*)malloc(static_cast<size_t>(n) * sizeof(Node));
	if (!_nodes) {
		std::cerr << "PollAllocator::Init(): failed to allocate nodes" << std::endl;
		free(_address);
		_address = nullptr;
		return false;
	}

	_head = 0;
	for (int i = 0; i < n; i++)
	{
		_nodes[i].free = true;
		if (i == n - 1) _nodes[i].next = -1;
		else _nodes[i].next = i + 1;
	}

	return true;
}

void *PoolAllocator::Request()
{

	return nullptr;
}

bool PoolAllocator::Free(void *element)
{
	if (element == nullptr) {
		std::cerr << "PoolAllocator::Free(): pointer is nullptr" << std::endl;
		return false;
	}

	// Casting to char pointer to allow for byte-wise arithmetics
	char* startAddress = static_cast<char*>(_address);
	char* offsetAddress = static_cast<char*>(element);
	ptrdiff_t byteDiff = offsetAddress - startAddress;

	// Bounds safety check
	if (byteDiff < 0 || byteDiff % _size != 0 || byteDiff >= _n * _size) {
		std::cerr << "PoolAllocator::Free(): pointer not in pool" << std::endl;
		return false;
	}

	int index = byteDiff / _size;

	if (_nodes[index].free) {
		std::cerr << "PoolAllocator::Free(): memory is already free" << std::endl;
		return false;
	}

	_nodes[index].free = true;
	_nodes[index].next = _head;
	_head = index;

	return true;
}
