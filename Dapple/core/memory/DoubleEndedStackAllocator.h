#pragma once

#include "CustomAllocation.h"

class DoubleEndedStackAllocator
{
public:
	// Stack marker: Represents the current top of the
	// stack. You can only roll back to a marker, not to
	// arbitrary locations within the stack.
	typedef uint32_t Marker;

	typedef unsigned char byte;

	// Constructs a stack allocator with the given total size
	explicit DoubleEndedStackAllocator(uint32_t stackSize_bytes);

	~DoubleEndedStackAllocator();

	// Allocates a new block of the given size from stack
	void* allocUpper(uint32_t size_bytes);
	void* allocUpperAligned(size_t bytes, size_t align);
	void* allocLower(uint32_t size_bytes);
	void* allocLowerAligned(size_t bytes, size_t align);

	// Returns a marker to the current stack top
	Marker getUpperMarker();
	Marker getLowerMarker();

	// Rolls the stack back to a previous marker
	void freeUpperToMarker(Marker marker);
	void freeLowerToMarker(Marker marker);

	// Clears the entire stack (rolls the stack back to zero)
	void clear();
	void clearUpper();
	void clearLower();

private:
	byte* m_memory;
	uint32_t m_size;
	Marker m_upper;
	Marker m_lower;
};