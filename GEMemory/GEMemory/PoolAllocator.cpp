#include "PoolAllocator.h"

#include <malloc.h>

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

	return false;
}
