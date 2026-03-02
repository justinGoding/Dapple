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

	void* Alloc(size_t size);
	void* AllocAligned(size_t bytes, size_t align);
	void Dealloc(void* chunk, size_t size);

	void FreeAlignedPool();

private:
	size_t m_ChunksPerBlock;
	Chunk* m_Head = nullptr;
	std::vector<void*> m_blockAddresses;

	Chunk* AllocateBlock(size_t chunk_size);
	Chunk* AllocateAlignedBlock(size_t chunk_size, size_t align);
};