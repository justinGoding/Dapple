#pragma once

#include <cstdint>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <functional>
#include <utility>

#ifdef _WIN64
#define NOMINMAX
#include <Windows.h>
#endif

#define WIN32_LEAN_AND_MEAN	// Exclude rarely used stuff from Windows headers

#define internal_linkage static
#define local_persist static
#define global_var static
#define non_instanced static

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

inline uint32 safeTruncateUInt64(uint64 val)
{
	assert(val <= UINT32_MAX);
	uint32 result = (uint32)val;
	return result;
}