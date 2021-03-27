#include "TileSet.h"

Bitmap& TileSet::GetBitmap()
{
	return this->tileSet;
}

Dim TileSet::GetTileSetWidth() const
{
	return this->cols;
}

Dim TileSet::GetTileSetHeight() const
{
	return this->rows;
}

bool TileSet::LoadTileSet(const char* path)
{
	this->tileSet = BitmapLoader::GetSigleton().LoadBitmap(path);
	if (!this->tileSet) 
	{
		std::cerr << "Failed to load the main Tile Set\n";
		return false;
	}

	this->rows = al_get_bitmap_height(this->tileSet) / TILE_HEIGHT;
	this->cols = al_get_bitmap_width(this->tileSet) / TILE_WIDTH;

	return true;
}

void TileSet::PutTile(Dim destX, Dim destY, Index index) const
{
	//We have an negative index
	if (index == USHRT_MAX)
		return;

	assert(this->tileSet != al_get_target_bitmap());

	al_draw_bitmap_region(this->tileSet, TileX(index), TileY(index), TILE_WIDTH, TILE_HEIGHT, destX, destY, 0);
}
