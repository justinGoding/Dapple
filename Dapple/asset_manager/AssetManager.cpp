#include "AssetManager.h"

#include "..\core\fileIO\Files.h"

template <typename T>
void AssetManager::loadAsset(string filepath, T* buffer)
{
	size_t bytesRead = 0;
	syncReadFile(filepath, reinterpret_cast<byte*>(buffer), sizeof(*buffer), bytesRead);
}

template <typename T>
void AssetManager::asyncLoadAsset(string filepath, T* buffer, uint8 priority)
{
	m_queue.push(assetTicket(filepath, reinterpret_cast<byte*>(buffer), priority)
}

void AssetManager::_load_assets()
{
	while (m_runLoadingThread)
	{
		std::optional<assetTicket> item = m_queue.pop();

		if (!item) continue;

		assetTicket ticket = item.value();

		size_t bytesRead = 0;
		syncReadFile(ticket.filepath, ticket.buffer, sizeof(*ticket.buffer), bytesRead);
	}
}
