#pragma once

#include "..\Core.h"
#include "..\containers\String.h"

inline bool syncReadFile(string filePath, byte* buffer, size_t bufferSize, size_t& rBytesRead)
{
	FILE* handle = fopen(filePath, "rb");
	if (handle)
	{
		// BLOCK here until all data has been read
		size_t bytesRead = fread(buffer, bufferSize, 1, handle);

		int err = ferror(handle); // get error if any

		fclose(handle);

		if (0 == err)
		{
			rBytesRead = bytesRead;
			return true;
		}

		const char* errMessage = strerror(errno);
		size_t newsize = strlen(errMessage) + 1;
		wchar_t* wcstring = new wchar_t[newsize];
		mbstowcs(wcstring, errMessage, newsize);
		MessageBoxW(NULL, wcstring, L"Dapple", MB_ICONERROR);
		rBytesRead = 0;
		return false;
	}
}