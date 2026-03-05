#include "StackAllocator.h"

typedef uint32_t Marker;
typedef unsigned char byte;

// Constructs a stack allocator with the given total size
StackAllocator::StackAllocator(uint32_t stackSize_bytes)
{
	m_memory = new byte[stackSize_bytes];
	m_top = 0;
	m_size = stackSize_bytes;
}

StackAllocator::~StackAllocator()
{
	delete[] m_memory;
}

// Allocates a new block of the given size from stack top
void* StackAllocator::alloc(uint32_t size_bytes)
{
	// Move the marker up by the size of the allocation
	m_top += size_bytes;
	// Ensure that the allocation does not exceed the allotted memory
	if (m_top > m_size)
	{
		m_top -= size_bytes;
		return nullptr;
	}
	return &m_memory[m_top - size_bytes];
}

// Aligned allocation function. IMPORTANT: 'align'
// must be a power of 2 (typically 4, 8, or 16)
void* StackAllocator::allocAligned(size_t bytes, size_t align)
{
	// Determine worst case number of bytes needed
	size_t worst_case_bytes = bytes + align - 1;

	// Allocate unaligned block
	void* p_raw_mem = alloc(worst_case_bytes);

	// Align the block
	return mem::alignPointer(p_raw_mem, align);
}

// Returns a marker to the current stack top
Marker StackAllocator::getMarker()
{
	return m_top;
}

// Rolls the stack back to a previous marker
void StackAllocator::freeToMarker(Marker marker)
{
	m_top = marker;
}

// Clears the entire stack (rolls the stack back to zero)
void StackAllocator::clear()
{
	m_top = 0;
}