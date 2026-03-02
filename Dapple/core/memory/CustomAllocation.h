#pragma once

#include <cstdlib>
#include <cassert>
#include <cstdint>

//void* operator new(size_t size)
//{
//	return malloc(size * sizeof(char));
//}
//
//void operator delete (void* mem)
//{
//	free(mem);
//}
//
//void* operator new[](size_t size)
//{
//	return malloc(size * sizeof(char));
//}
//
//void operator delete[](void* mem)
//{
//	free(mem);
//}

namespace mem {
	// Shift the given address upwards if/as necessary to
	// ensure it is aligned to the given number of bytes
	inline uintptr_t alignAddress(uintptr_t addr, size_t align)
	{
		const size_t mask = align - 1;
		assert((align & mask) == 0);
		return (addr + mask) & ~mask;
	}

	// Shift the given address upwards if/as necessarry to
	// ensure it is aligned to the given number of bytes
	template<typename T>
	inline T* alignPointer(T* ptr, size_t align)
	{
		const uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
		const uintptr_t addr_aligned = alignAddress(addr, align);
		return reinterpret_cast<T*>(addr_aligned);
	}

	// Aligned allocation function. IMPORTANT: 'align'
	// must be a power of 2 (typically 4, 8, or 16)
	inline void* allocAligned(size_t bytes, size_t align)
	{
		// Determine the worse case number of bytes needed
		size_t actual_bytes = bytes + align;

		// Allocate unaligned block
		uint8_t* p_raw_mem = new uint8_t[actual_bytes];

		// Align the block. If no alignment occurred,
		// shift it up the full 'align' bytes so we
		// always have room to store the shift.
		uint8_t* p_aligned_mem = alignPointer(p_raw_mem, align);
		if (p_aligned_mem == p_raw_mem)
			p_aligned_mem += align;

		// Determine the shift, and store it
		// (This works for up to 256-byte alignments
		ptrdiff_t shift = p_aligned_mem - p_raw_mem;
		assert(shift > 0 && shift <= 256);
		p_aligned_mem[-1] = static_cast<uint8_t>(shift & 0xFF);

		return p_aligned_mem;
	}

	inline void freeAligned(void* p_mem)
	{
		if (p_mem)
		{
			// Convert to uint8_t
			uint8_t* p_aligned_mem = reinterpret_cast<uint8_t*>(p_mem);

			// Extract the shift
			ptrdiff_t shift = p_aligned_mem[-1];
			if (shift == 0)
				shift = 256;

			// Back up to the actual allocated address
			// and array-delete it
			uint8_t* p_raw_mem = p_aligned_mem - shift;
			delete[] p_raw_mem;
		}
	}
}