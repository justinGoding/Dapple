#pragma once

#include "..\core\Core.h"

enum game_asset_id
{
	guid_count,
};

struct loaded_bitmap
{

};

class AssetManager
{
public:
	AssetManager() {}

	void loadAsset(game_asset_id id);

	loaded_bitmap* getBitmap(game_asset_id id) { return &m_bitmaps[id]; }

private:
	loaded_bitmap m_bitmaps[guid_count];
};