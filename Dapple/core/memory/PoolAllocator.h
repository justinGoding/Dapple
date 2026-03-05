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
	PoolAllocator();
	PoolAllocator(size_t chunks_per_block);
	~PoolAllocator();

	void* alloc(size_t size);
	void* allocAligned(size_t bytes, size_t align);
	void dealloc(void* chunk, size_t size);

	void freeAlignedPool();

private:
	size_t m_chunksPerBlock;
	Chunk* m_head = nullptr;
	std::vector<void*> m_blockAddresses;

	Chunk* allocateBlock(size_t chunk_size);
	Chunk* allocateAlignedBlock(size_t chunk_size, size_t align);
};