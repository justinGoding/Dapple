#pragma once

#include <thread>

#include "..\core\Core.h"
#include "..\core\containers\String.h"
#include "..\core\containers\ThreadSafeMaxHeap.h"

enum game_asset_id
{
	guid_count,
};

struct assetTicket
{
	assetTicket(string filepath, uint8 priority)
		: filepath(filepath), priority(priority)
	{
	}

	string filepath;
	byte* buffer;
	uint8 priority;
};

class AssetManager
{
public:
	AssetManager() : m_queue(ThreadSafeMaxHeap<assetTicket>()), m_loadingThread(std::thread(_load_assets))
	{
		m_loadingThread.detach();
	}

	~AssetManager()
	{
		m_runLoadingThread = false;
		m_queue.close();
	}

	template <typename T>
	void loadAsset(string filepath, T* buffer);

	template <typename T>
	void asyncLoadAsset(string filepath, T* buffer, uint8 priority = 1);

private:
	void _load_assets();

private:
	ThreadSafeMaxHeap<assetTicket> m_queue;
	std::thread m_loadingThread;

	volatile bool m_runLoadingThread = true;
};