#pragma once

#include "..\Core.h"
#include "..\containers\String.h"

bool syncReadFile(string filePath, uint8* buffer, size_t bufferSize, size_t& rBytesRead)
{
	FILE* handle = fopen(filePath, "rb");
	if (handle)
	{
		// BLOCK here until all data has been read
		size_t bytesRead = fread(buffer, 1, bufferSize, handle);

		int err = ferror(handle); // get error if any

		fclose(handle);

		if (0 == err)
		{
			rBytesRead = bytesRead;
			return true;
		}
		rBytesRead = 0;
		return false;
	}
}

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