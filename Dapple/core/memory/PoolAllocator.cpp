#include "PoolAllocator.h"

#include <iostream>

PoolAllocator::PoolAllocator()
	: m_chunksPerBlock(4)
{
}

PoolAllocator::PoolAllocator(size_t chunks_per_block)
	: m_chunksPerBlock(chunks_per_block)
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
// IMPORTANT: size must be greater than sizeof(void*)
void* PoolAllocator::alloc(size_t size)
{
	assert(size >= sizeof(void*));

	if (m_head == nullptr)
	{
		m_head = allocateBlock(size);
	}

	// The return value is the current position of
	// the allocation pointer

	Chunk* free_chunk = m_head;

	// Advance the allocation pointer to the next chunk
	// When no chunks left, the m_Head will be set to the nullptr and
	// this will cause the allocation of a new block on the next request

	m_head = m_head->next;

	return free_chunk;
}

// Aligned allocation function. IMPORTANT: 'align'
// must be a power of 2 (typically 4, 8, or 16)
void* PoolAllocator::allocAligned(size_t bytes, size_t align)
{
	if (m_head == nullptr)
	{
		m_head = allocateAlignedBlock(bytes, align);
	}

	// The return value is the current position of
	// the allocation pointer

	Chunk* free_chunk = m_head;

	// Advance the allocation pointer to the next chunk
	// When no chunks left, the m_Head will be set to the nullptr and
	// this will cause the allocation of a new block on the next request

	m_head = m_head->next;

	return free_chunk;
}

// Puts the chunk into the front of the chunks list
void PoolAllocator::dealloc(void* chunk, size_t size)
{
	// The freed chunk's next pointer points to the
	// current allocation pointer

	reinterpret_cast<Chunk*>(chunk)->next = m_head;

	// And the allocation pointer is now set
	// to the returned (free) chunk

	m_head = reinterpret_cast<Chunk*>(chunk);
}

// Allocates a new block from the OS
// Returns a Chunk pointer set to the beginning of the block
Chunk* PoolAllocator::allocateBlock(size_t chunk_size)
{
	size_t block_size = m_chunksPerBlock * chunk_size;

	// The first chunk of the new block
	void* block_begin = malloc(block_size);
	Chunk* first_chunk = reinterpret_cast<Chunk*>(block_begin);
	m_blockAddresses.pushBack(block_begin);

	// Once the block is allocated, we need to chain all
	// the chunks in this block

	Chunk* chunk = first_chunk;

	for (int i = 0; i < m_chunksPerBlock - 1; ++i)
	{
		chunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + chunk_size);
		chunk = chunk->next;
	}

	chunk->next = nullptr;

	return first_chunk;
}

// Allocates a new block from the OS
// Returns a Chunk pointer set to the beginning of the block
Chunk* PoolAllocator::allocateAlignedBlock(size_t chunk_size, size_t align)
{
	size_t block_size = m_chunksPerBlock * chunk_size;

	// The first chunk of the new block
	void* block_begin = mem::allocAligned(block_size, align);
	Chunk* first_chunk = reinterpret_cast<Chunk*>(block_begin);
	m_blockAddresses.pushBack(block_begin);

	// Once the block is allocated, we need to chain all
	// the chunks in this block

	Chunk* chunk = first_chunk;

	for (int i = 0; i < m_chunksPerBlock - 1; ++i)
	{
		chunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + chunk_size);
		chunk = chunk->next;
	}

	chunk->next = nullptr;

	return first_chunk;
}

void PoolAllocator::freeAlignedPool()
{
	for (auto block : m_blockAddresses)
		mem::freeAligned(block);
	m_blockAddresses.clear();
}