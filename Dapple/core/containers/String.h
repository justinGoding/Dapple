#pragma once

#include <string>

#include "..\xxh64.h"

typedef uint64_t sid;
typedef const char* string;
typedef const WCHAR* wstring;

inline sid operator"" _sid(string s, size_t len)
{
	return xxh64::hash(s, len, 1);
}

constexpr sid hash_str(string s)
{
	return xxh64::hash(s, std::char_traits<char>::length(s), 1);
}

#ifdef _DEBUG
#define SID(str) StringID(str, hash_str(str))
#else
#define SID(str) StringID(hash_str(str))
#endif


struct StringID
{
#ifdef _DEBUG
	StringID() : str(""), strID(0) {}
	StringID(string s, sid id) : str(s), strID(id) {}

	string str = nullptr;

	operator string() const { return str; }
#else
	StringID() : strID(0) {}
	StringID(sid id) : strID(id) {}
#endif
	sid strID = 0;

	operator sid() const { return strID; }
};