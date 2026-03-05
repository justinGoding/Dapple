#include "DoubleEndedStackAllocator.h"

typedef uint32_t Marker;
typedef unsigned char byte;

// Constructs a double ended stack allocator with the given total size
DoubleEndedStackAllocator::DoubleEndedStackAllocator(uint32_t stackSize_bytes)
{
	m_memory = new byte[stackSize_bytes];
	m_size = stackSize_bytes;
	m_upper = 0;
	m_lower = 0;
}

DoubleEndedStackAllocator::~DoubleEndedStackAllocator()
{
	delete[] m_memory;
}

// Allocates a new block of the given size from upper stack
void* DoubleEndedStackAllocator::allocUpper(uint32_t size_bytes)
{
	// Move the marker up by the size of the allocation
	m_upper += size_bytes;
	// Ensure that the allocation does not exceed the allotted memory
	if (m_upper + m_lower > m_size)
	{
		m_upper -= size_bytes;
		return nullptr;
	}
	return &m_memory[m_size - m_upper];
}

// Aligned allocation function. IMPORTANT: 'align'
// must be a power of 2 (typically 4, 8, or 16)
void* DoubleEndedStackAllocator::allocUpperAligned(size_t bytes, size_t align)
{
	// Determine worst case number of bytes needed
	size_t worst_case_bytes = bytes + align - 1;

	// Allocate unaligned block
	void* p_raw_mem = allocUpper(worst_case_bytes);

	// Align the block
	return mem::alignPointer(p_raw_mem, align);
}

// Allocates a new block of the given size from lower stack
void* DoubleEndedStackAllocator::allocLower(uint32_t size_bytes)
{
	// Move the marker up by the size of the allocation
	m_lower += size_bytes;
	// Ensure that the allocation does not exceed the allotted memory
	if (m_upper + m_lower > m_size)
	{
		m_lower -= size_bytes;
		return nullptr;
	}
	return &m_memory[m_lower - size_bytes];
}

// Aligned allocation function. IMPORTANT: 'align'
// must be a power of 2 (typically 4, 8, or 16)
void* DoubleEndedStackAllocator::allocLowerAligned(size_t bytes, size_t align)
{
	// Determine worst case number of bytes needed
	size_t worst_case_bytes = bytes + align - 1;

	// Allocate unaligned block
	void* p_raw_mem = allocLower(worst_case_bytes);

	// Align the block
	return mem::alignPointer(p_raw_mem, align);
}

// Returns a marker to the current upper stack
Marker DoubleEndedStackAllocator::getUpperMarker()
{
	return m_upper;
}

// Returns a marker to the current lower stack
Marker DoubleEndedStackAllocator::getLowerMarker()
{
	return m_lower;
}

// Rolls the stack back to a previous marker
void DoubleEndedStackAllocator::freeUpperToMarker(Marker marker)
{
	m_upper = marker;
}

// Rolls the stack back to a previous marker
void DoubleEndedStackAllocator::freeLowerToMarker(Marker marker)
{
	m_lower = marker;
}

// Clears the entire stack (rolls the stack back to zero)
void DoubleEndedStackAllocator::clear()
{
	m_upper = 0;
	m_lower = 0;
}

// Clears the upper stack
void DoubleEndedStackAllocator::clearUpper()
{
	m_upper = 0;
}

// Clears the lower stack
void DoubleEndedStackAllocator::clearLower()
{
	m_lower = 0;
}