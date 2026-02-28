#include "StackAllocator.h"

typedef uint32_t Marker;
typedef unsigned char byte;

// Constructs a stack allocator with the given total size
StackAllocator::StackAllocator(uint32_t stackSize_bytes)
{
	m_Memory = new byte[stackSize_bytes];
	m_Top = 0;
	m_Size = stackSize_bytes;
}

StackAllocator::~StackAllocator()
{
	delete[] m_Memory;
}

// Allocates a new block of the given size from stack top
void* StackAllocator::Alloc(uint32_t size_bytes)
{
	// Move the marker up by the size of the allocation
	m_Top += size_bytes;
	// Ensure that the allocation does not exceed the allotted memory
	if (m_Top > m_Size)
	{
		m_Top -= size_bytes;
		return nullptr;
	}
	return &m_Memory[m_Top];
}

// Returns a marker to the current stack top
Marker StackAllocator::GetMarker()
{
	return m_Top;
}

// Rolls the stack back to a previous marker
void StackAllocator::FreeToMarker(Marker marker)
{
	m_Top = marker;
}

// Clears the entire stack (rolls the stack back to zero)
void StackAllocator::Clear()
{
	m_Top = 0;
}