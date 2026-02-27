#pragma once

#include <iostream>
#include <Windows.h>
#include <sstream>

// Custom stream buffer to redirect cout to OutputDebugString
class DebugStreamBuffer : public std::streambuf
{
protected:
	int overflow(int c) override
	{
		if (c != EOF)
		{
			char buf[2] = { static_cast<char>(c), '\0' };
			OutputDebugStringA(buf); // Send to debugger
		}
		return c;
	}

	std::streamsize xsputn(const char* s, std::streamsize count) override
	{
		OutputDebugStringA(s); // Send entire string to debugger
		return count;
	}

	int sync() override
	{
		return 0; // No-op; OutputDebugString doesn't buffer
	}
};

// Initialize the custom streambuf once at app startup
extern void RedirectCoutToDebugger()
{
	static DebugStreamBuffer debugBuffer;
	std::cout.rdbuf(&debugBuffer); // replace cout's buffer
}