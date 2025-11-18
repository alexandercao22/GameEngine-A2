#include "PoolAllocator.h"

#include <malloc.h>
#include <iostream>

bool PoolAllocator::Expand()
{

	return false;
}

PoolAllocator::~PoolAllocator()
{

}

bool PoolAllocator::Init(int n, int size)
{
	_n = n;
	_size = size;

	_address = malloc(static_cast<size_t>(n * size));
	_nodes = (Node*)malloc(static_cast<size_t>(n) * sizeof(Node));

	_head = 0;
	if (_nodes != nullptr)
	{
		for (int i = 0; i < n; i++)
		{
			_nodes[i].free = true;
			if (i == n - 1) _nodes[i].next = -1;
			else _nodes[i].next = i + 1;
		}
	}

	return _address != nullptr && _nodes != nullptr;
}

void *PoolAllocator::Request()
{

	return nullptr;
}

bool PoolAllocator::Free(void *element)
{
	if (element == nullptr) {
		std::cerr << "PoolAllocator::Free(): pointer is nullptr";
		return false;
	}

	// Casting to char pointer to allow for byte-wise arithmetics
	char* startAddress = static_cast<char*>(_address);
	char* offsetAddress = static_cast<char*>(element);
	ptrdiff_t byteDiff = offsetAddress - startAddress;

	// Bounds safety check
	if (byteDiff < 0 || byteDiff % _size != 0 || byteDiff >= _n * _size) {
		std::cerr << "PoolAllocator::Free(): pointer not in pool";
		return false;
	}

	int prevHead = _head;
	_head = byteDiff / _size;
	_nodes[_head].free = true;
	_nodes[_head].next = prevHead;

	return true;
}
