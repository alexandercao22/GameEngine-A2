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
	_nodes = (int*)malloc(static_cast<size_t>(n * sizeof(int)));

	if (_nodes != nullptr)
	{
		for (int i = 0; i < n; i++)
		{
			_nodes[i] = -1;
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
