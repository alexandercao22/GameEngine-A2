#include "PoolAllocator.h"

bool PoolAllocator::Expand()
{

	return false;
}

PoolAllocator::~PoolAllocator()
{

}

bool PoolAllocator::Init(int n, int size)
{

}

void *PoolAllocator::Request()
{

	return nullptr;
}

bool PoolAllocator::Free(void *element)
{

	return false;
}
