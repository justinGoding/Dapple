#pragma once

#include <cstdlib>

void* operator new(size_t size)
{
	return malloc(size * sizeof(char));
}

void operator delete (void* mem)
{
	free(mem);
}

void* operator new[](size_t size)
{
	return malloc(size * sizeof(char));
}

void operator delete[](void* mem)
{
	free(mem);
}