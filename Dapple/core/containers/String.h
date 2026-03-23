#pragma once

#include "..\xxh64.h"

#ifdef _DEBUG
	#define SID(str) (StringID(str, str_sid))
#elif
	#define SID(str) (StringID(str_sid))
#endif

inline uint64_t operator"" _sid(const char* s, size_t len)
{
	return xxh64::hash(s, len, 1);
}

struct StringID
{
#ifdef _DEBUG
	StringID(const char* s, uint64_t id) : str(s), strID(id) {}

	const char* str = nullptr;
#elif
	StringID(uint64_t id) : strID(id) {}
#endif
	uint64_t strID = 0;
};