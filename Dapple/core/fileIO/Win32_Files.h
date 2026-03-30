#pragma once

#include "..\Core.h"
#include "..\containers\String.h"


#ifdef _DEBUG

struct debug_readFileResult
{
	uint32 ContentsSize;
	void* Contents;
};

inline void DEBUG_FreeFileMemory(void* memory)
{
	if (memory)
	{
		VirtualFree(memory, NULL, MEM_RELEASE);
	}
}

inline debug_readFileResult DEBUG_ReadEntireFile(string filename)
{
	debug_readFileResult result = {};

	HANDLE fileHandle = CreateFileA(
		filename,			// file path
		GENERIC_READ,		// desired access
		FILE_SHARE_READ,	// share mode
		NULL,				// security attributes
		OPEN_EXISTING,		// creation disposition
		NULL,				// flags and attributes
		NULL				// template file
	);

	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER fileSize;
		if (GetFileSizeEx(fileHandle, &fileSize))
		{
			uint32 fileSize32 = safeTruncateUInt64(fileSize.QuadPart);

			result.Contents = VirtualAlloc(NULL, fileSize32, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			if (result.Contents)
			{
				DWORD bytesRead;
				if (ReadFile(fileHandle,result.Contents, fileSize32, &bytesRead, NULL) 
					&& fileSize32 == bytesRead)
				{
					result.ContentsSize = fileSize32;
				}
				else
				{
					// TODO: logging
					DEBUG_FreeFileMemory(result.Contents);
					result.Contents = nullptr;
				}
			}
			else
			{
				// TODO: logging
			}
		}
		else
		{
			// TODO: logging
		}

		CloseHandle(fileHandle);
	}
	else
	{
		// TODO: logging
	}

	return result;
}

inline bool DEBUG_WriteEntireFile(string filename, uint32 memory_size, void* memory)
{
	bool result = false;

	HANDLE fileHandle = CreateFileA(
		filename,			// file path
		GENERIC_WRITE,		// desired access
		NULL,				// share mode
		NULL,				// security attributes
		CREATE_ALWAYS,		// creation disposition
		NULL,				// flags and attributes
		NULL				// template file
	);

	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		DWORD bytesWritten;
		if (WriteFile(fileHandle, memory, memory_size, &bytesWritten, NULL))
		{
			// NOTE: File read successfully
			result = (bytesWritten == memory_size);
		}
		else
		{
			// TODO: logging
		}
		
		CloseHandle(fileHandle);
	}
	else
	{
		// TODO: logging
	}

	return result;
}

#endif

//AsyncRequestHandle g_hRequest; // async I/O request handle
//uint8_t g_asyncBuffer[512]; // input buffer
//
//inline void asyncReadComplete(AsyncRequestHandle hRequest)
//{
//	if (hRequest == g_hRequest && asyncWasSuccessful(hRequest))
//	{
//		// The data is now present in g_asyncBuffer[] and
//		// can be used. Query for the number of bytes
//		// actually read:
//		size_t bytes = asyncGetBytesReadOrWritten(hRequest);
//
//		char msg[256];
//		snprintf(msg, sizeof(msg),
//			"async success, read %u bytes\n", bytes);
//		std::cout << msg << std::endl;
//	}
//}
//
//inline bool asyncStreamFile(string filepath)
//{
//	AsyncFileHandle hFile = asyncOpen(filepath);
//
//	if (hFile)
//	{
//		// This function requests an I/O read
//		// returns immediately (non-blocking)
//		g_hRequest = asyncReadFile(
//			hFile,					// file handle
//			g_asyncBuffer,			// input buffer
//			sizeof(g_asyncBuffer),	// size of buffer
//			asyncReadComplete		// callback function
//		);
//	}
//}