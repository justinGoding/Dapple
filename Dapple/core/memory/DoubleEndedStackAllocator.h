#pragma once

#include <cstdint>

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

	// Allocates a new block of the given size from stack
	void* AllocUpper(uint32_t size_bytes);
	void* AllocLower(uint32_t size_bytes);

	// Returns a marker to the current stack top
	Marker GetUpperMarker();
	Marker GetLowerMarker();

	// Rolls the stack back to a previous marker
	void FreeUpperToMarker(Marker marker);
	void FreeLowerToMarker(Marker marker);

	// Clears the entire stack (rolls the stack back to zero)
	void Clear();
	void ClearUpper();
	void ClearLower();

private:
	byte* m_Memory;
	uint32_t m_Size;
	Marker m_Upper;
	Marker m_Lower;
};