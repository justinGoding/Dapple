#pragma once

#include "CustomAllocation.h"

class StackAllocator
{
public:
	// Stack marker: Represents the current top of the
	// stack. You can only roll back to a marker, not to
	// arbitrary locations within the stack.
	typedef uint32_t Marker;

	typedef unsigned char byte;

	StackAllocator() {}

	// Constructs a stack allocator with the given total size
	explicit StackAllocator(uint32_t stackSize_bytes);

	~StackAllocator();

	// Allocates a new block of the given size from stack top
	void* alloc(uint32_t size_bytes);

	void* allocAligned(size_t size_bytes, size_t align);

	// Returns a marker to the current stack top
	Marker getMarker();

	// Rolls the stack back to a previous marker
	void freeToMarker(Marker marker);

	// Clears the entire stack (rolls the stack back to zero)
	void clear();

private:
	byte* m_memory = nullptr;
	Marker m_top = 0;
	size_t m_size = 0;
};