#include "PoolAllocator.h"

#include <iostream>

PoolAllocator::PoolAllocator(size_t chunks_per_block)
	: m_ChunksPerBlock(chunks_per_block)
{
}

PoolAllocator::~PoolAllocator()
{
	for (auto block : m_blockAddresses)
		free(block);
}

// Returns the first free chunk in the block
// If there are no chunks left in the block,
// allocates a new block
void* PoolAllocator::Alloc(size_t size)
{
	if (m_Head == nullptr)
	{
		m_Head = AllocateBlock(size);
	}

	// The return value is the current position of
	// the allocation pointer

	Chunk* free_chunk = m_Head;

	// Advance the allocation pointer to the next chunk
	// When no chunks left, the m_Head will be set to the nullptr and
	// this will cause the allocation of a new block on the next request

	m_Head = m_Head->next;

	return free_chunk;
}

// Aligned allocation function. IMPORTANT: 'align'
// must be a power of 2 (typically 4, 8, or 16)
void* PoolAllocator::AllocAligned(size_t bytes, size_t align)
{
	if (m_Head == nullptr)
	{
		m_Head = AllocateAlignedBlock(bytes, align);
	}

	// The return value is the current position of
	// the allocation pointer

	Chunk* free_chunk = m_Head;

	// Advance the allocation pointer to the next chunk
	// When no chunks left, the m_Head will be set to the nullptr and
	// this will cause the allocation of a new block on the next request

	m_Head = m_Head->next;

	return free_chunk;
}

// Puts the chunk into the front of the chunks list
void PoolAllocator::Dealloc(void* chunk, size_t size)
{
	// The freed chunk's next pointer points to the
	// current allocation pointer

	reinterpret_cast<Chunk*>(chunk)->next = m_Head;

	// And the allocation pointer is now set
	// to the returned (free) chunk

	m_Head = reinterpret_cast<Chunk*>(chunk);
}

// Allocates a new block from the OS
// Returns a Chunk pointer set to the beginning of the block
Chunk* PoolAllocator::AllocateBlock(size_t chunk_size)
{
	size_t block_size = m_ChunksPerBlock * chunk_size;

	// The first chunk of the new block
	void* block_begin = malloc(block_size);
	Chunk* first_chunk = reinterpret_cast<Chunk*>(block_begin);
	m_blockAddresses.push_back(block_begin);

	// Once the block is allocated, we need to chain all
	// the chunks in this block

	Chunk* chunk = first_chunk;

	for (int i = 0; i < m_ChunksPerBlock - 1; ++i)
	{
		chunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + chunk_size);
		chunk = chunk->next;
	}

	chunk->next = nullptr;

	return first_chunk;
}

// Allocates a new block from the OS
// Returns a Chunk pointer set to the beginning of the block
Chunk* PoolAllocator::AllocateAlignedBlock(size_t chunk_size, size_t align)
{
	size_t block_size = m_ChunksPerBlock * chunk_size;

	// The first chunk of the new block
	void* block_begin = mem::allocAligned(block_size, align);
	Chunk* first_chunk = reinterpret_cast<Chunk*>(block_begin);
	m_blockAddresses.push_back(block_begin);

	// Once the block is allocated, we need to chain all
	// the chunks in this block

	Chunk* chunk = first_chunk;

	for (int i = 0; i < m_ChunksPerBlock - 1; ++i)
	{
		chunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + chunk_size);
		chunk = chunk->next;
	}

	chunk->next = nullptr;

	return first_chunk;
}

void PoolAllocator::FreeAlignedPool()
{
	for (auto block : m_blockAddresses)
		mem::freeAligned(block);
	m_blockAddresses.clear();
}