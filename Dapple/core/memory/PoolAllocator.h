#pragma once

#include "CustomAllocation.h"

#include <vector>

// A single element of memory in the pool
struct Chunk
{
	Chunk* next;
};

class PoolAllocator
{
public:
	PoolAllocator(size_t chunks_per_block);
	~PoolAllocator();

	void* allocate(size_t size);
	void deallocate(void* chunk, size_t size);

private:
	size_t m_ChunksPerBlock;
	Chunk* m_Head = nullptr;
	std::vector<void*> m_blockAddresses;

	Chunk* AllocateBlock(size_t size);
};