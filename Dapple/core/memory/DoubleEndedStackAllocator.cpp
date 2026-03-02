#include "DoubleEndedStackAllocator.h"

typedef uint32_t Marker;
typedef unsigned char byte;

// Constructs a double ended stack allocator with the given total size
DoubleEndedStackAllocator::DoubleEndedStackAllocator(uint32_t stackSize_bytes)
{
	m_Memory = new byte[stackSize_bytes];
	m_Size = stackSize_bytes;
	m_Upper = 0;
	m_Lower = 0;
}

DoubleEndedStackAllocator::~DoubleEndedStackAllocator()
{
	delete[] m_Memory;
}

// Allocates a new block of the given size from upper stack
void* DoubleEndedStackAllocator::AllocUpper(uint32_t size_bytes)
{
	// Move the marker up by the size of the allocation
	m_Upper += size_bytes;
	// Ensure that the allocation does not exceed the allotted memory
	if (m_Upper + m_Lower > m_Size)
	{
		m_Upper -= size_bytes;
		return nullptr;
	}
	return &m_Memory[m_Size - m_Upper];
}

// Aligned allocation function. IMPORTANT: 'align'
// must be a power of 2 (typically 4, 8, or 16)
void* DoubleEndedStackAllocator::AllocUpperAligned(size_t bytes, size_t align)
{
	// Determine worst case number of bytes needed
	size_t worst_case_bytes = bytes + align - 1;

	// Allocate unaligned block
	void* p_raw_mem = AllocUpper(worst_case_bytes);

	// Align the block
	return mem::alignPointer(p_raw_mem, align);
}

// Allocates a new block of the given size from lower stack
void* DoubleEndedStackAllocator::AllocLower(uint32_t size_bytes)
{
	// Move the marker up by the size of the allocation
	m_Lower += size_bytes;
	// Ensure that the allocation does not exceed the allotted memory
	if (m_Upper + m_Lower > m_Size)
	{
		m_Lower -= size_bytes;
		return nullptr;
	}
	return &m_Memory[m_Lower - size_bytes];
}

// Aligned allocation function. IMPORTANT: 'align'
// must be a power of 2 (typically 4, 8, or 16)
void* DoubleEndedStackAllocator::AllocLowerAligned(size_t bytes, size_t align)
{
	// Determine worst case number of bytes needed
	size_t worst_case_bytes = bytes + align - 1;

	// Allocate unaligned block
	void* p_raw_mem = AllocLower(worst_case_bytes);

	// Align the block
	return mem::alignPointer(p_raw_mem, align);
}

// Returns a marker to the current upper stack
Marker DoubleEndedStackAllocator::GetUpperMarker()
{
	return m_Upper;
}

// Returns a marker to the current lower stack
Marker DoubleEndedStackAllocator::GetLowerMarker()
{
	return m_Lower;
}

// Rolls the stack back to a previous marker
void DoubleEndedStackAllocator::FreeUpperToMarker(Marker marker)
{
	m_Upper = marker;
}

// Rolls the stack back to a previous marker
void DoubleEndedStackAllocator::FreeLowerToMarker(Marker marker)
{
	m_Lower = marker;
}

// Clears the entire stack (rolls the stack back to zero)
void DoubleEndedStackAllocator::Clear()
{
	m_Upper = 0;
	m_Lower = 0;
}

// Clears the upper stack
void DoubleEndedStackAllocator::ClearUpper()
{
	m_Upper = 0;
}

// Clears the lower stack
void DoubleEndedStackAllocator::ClearLower()
{
	m_Lower = 0;
}