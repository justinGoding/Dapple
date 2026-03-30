#pragma once

#include <cstring>

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

	StackAllocator(StackAllocator& other);

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

	StackAllocator& operator=(const StackAllocator& rhs)
	{
		if (this != &rhs)
		{
			byte* new_stack = new byte[rhs.m_size];
			std::memcpy(new_stack, rhs.m_memory, rhs.m_size);

			if (m_memory)
				delete[] m_memory;

			m_memory = new_stack;
			m_top = rhs.m_top;
			m_size = rhs.m_size;
		}

		return *this;
	}

private:
	byte* m_memory = nullptr;
	Marker m_top = 0;
	size_t m_size = 0;
};