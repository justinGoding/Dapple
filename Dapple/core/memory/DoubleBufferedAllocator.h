#pragma once

#include "StackAllocator.h"

class DoubleBufferedAllocator
{
public:
	explicit DoubleBufferedAllocator(uint32_t stackSize_bytes)
	{
		m_curStack = 0;
		m_stack[0] = StackAllocator(stackSize_bytes);
		m_stack[1] = StackAllocator(stackSize_bytes);
	}

	void swapBuffers()
	{
		m_curStack = (uint32_t)!m_curStack;
	}

	void clearCurrentBuffer()
	{
		m_stack[m_curStack].Clear();
	}

	void* alloc(uint32_t nBytes)
	{
		return m_stack[m_curStack].Alloc(nBytes);
	}

private:
	uint32_t m_curStack;
	StackAllocator m_stack[2];
};