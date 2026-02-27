#pragma once

#include "CustomAllocation.h"

#include <cstdint>

class StackAllocator
{
public:
	// Stack marker: Represents the current top of the
	// stack. You can only roll back to a marker, not to
	// arbitrary locations within the stack.
	typedef uint32_t Marker;

	typedef unsigned char byte;

	// Constructs a stack allocator with the given total size
	explicit StackAllocator(uint32_t stackSize_bytes);

	~StackAllocator();

	// Allocates a new block of the given size from stack top
	void* Alloc(uint32_t size_bytes);

	// Returns a marker to the current stack top
	Marker GetMarker();

	// Rolls the stack back to a previous marker
	void FreeToMarker(Marker marker);

	// Clears the entire stack (rolls the stack back to zero)
	void Clear();

private:
	byte* m_Memory;
	Marker m_Top;
};