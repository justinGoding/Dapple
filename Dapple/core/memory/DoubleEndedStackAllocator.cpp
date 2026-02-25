#include "DoubleEndedStackAllocator.h"
#include "CustomAllocation.h"

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
	return &m_Memory[m_Lower];
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