#include "GridComputation.h"

GridComputation::GridComputation() 
{
	this->emptyTiles.push_back(MakeIndex(0, 0));
	this->emptyTiles.push_back(MakeIndex(0, 1));
	this->emptyTiles.push_back(MakeIndex(0, 2));
	this->emptyTiles.push_back(MakeIndex(0, 10));
	this->emptyTiles.push_back(MakeIndex(0, 11));
	this->emptyTiles.push_back(MakeIndex(0, 12));
	this->emptyTiles.push_back(MakeIndex(0, 13));

	this->emptyTiles.push_back(MakeIndex(1, 10));
	this->emptyTiles.push_back(MakeIndex(1, 11));
	this->emptyTiles.push_back(MakeIndex(1, 12));
	this->emptyTiles.push_back(MakeIndex(1, 13));

	this->emptyTiles.push_back(MakeIndex(2, 3));
	this->emptyTiles.push_back(MakeIndex(2, 4));
	this->emptyTiles.push_back(MakeIndex(2, 10));
	//this->emptyTiles.push_back(MakeIndex(2, 11));
	this->emptyTiles.push_back(MakeIndex(2, 12));
	//this->emptyTiles.push_back(MakeIndex(2, 13));

	this->emptyTiles.push_back(MakeIndex(3, 3));
	this->emptyTiles.push_back(MakeIndex(3, 4));
	this->emptyTiles.push_back(MakeIndex(3, 10));
	this->emptyTiles.push_back(MakeIndex(3, 11));
	this->emptyTiles.push_back(MakeIndex(3, 12));
	this->emptyTiles.push_back(MakeIndex(3, 13));

	this->emptyTiles.push_back(MakeIndex(4, 0));
	this->emptyTiles.push_back(MakeIndex(4, 1));
	this->emptyTiles.push_back(MakeIndex(4, 2));
	this->emptyTiles.push_back(MakeIndex(4, 3));
	this->emptyTiles.push_back(MakeIndex(4, 4));
	this->emptyTiles.push_back(MakeIndex(4, 5));
	this->emptyTiles.push_back(MakeIndex(4, 6));
	this->emptyTiles.push_back(MakeIndex(4, 7));

	this->emptyTiles.push_back(MakeIndex(5, 0));
	this->emptyTiles.push_back(MakeIndex(5, 1));
	this->emptyTiles.push_back(MakeIndex(5, 2));
	this->emptyTiles.push_back(MakeIndex(5, 3));
	this->emptyTiles.push_back(MakeIndex(5, 4));
	this->emptyTiles.push_back(MakeIndex(5, 5));
	this->emptyTiles.push_back(MakeIndex(5, 6));
	this->emptyTiles.push_back(MakeIndex(5, 7));

	this->emptyTiles.push_back(MakeIndex(6, 0));
	this->emptyTiles.push_back(MakeIndex(6, 1));
	this->emptyTiles.push_back(MakeIndex(6, 2));
	this->emptyTiles.push_back(MakeIndex(6, 3));
	this->emptyTiles.push_back(MakeIndex(6, 4));
	this->emptyTiles.push_back(MakeIndex(6, 5));
	this->emptyTiles.push_back(MakeIndex(6, 6));
	this->emptyTiles.push_back(MakeIndex(6, 7));

	this->emptyTiles.push_back(MakeIndex(7, 0));
	this->emptyTiles.push_back(MakeIndex(7, 1));
	this->emptyTiles.push_back(MakeIndex(7, 2));
	this->emptyTiles.push_back(MakeIndex(7, 3));
	this->emptyTiles.push_back(MakeIndex(7, 4));
	this->emptyTiles.push_back(MakeIndex(7, 5));
	this->emptyTiles.push_back(MakeIndex(7, 6));
	this->emptyTiles.push_back(MakeIndex(7, 7));

	this->emptyTiles.push_back(MakeIndex(12, 0));
	this->emptyTiles.push_back(MakeIndex(12, 1));
	this->emptyTiles.push_back(MakeIndex(12, 2));

	/*this->emptyTiles.push_back(MakeIndex(0, 0));
	this->emptyTiles.push_back(MakeIndex(0, 1));
	this->emptyTiles.push_back(MakeIndex(0, 2));
	this->emptyTiles.push_back(MakeIndex(0, 3));
	this->emptyTiles.push_back(MakeIndex(0, 4));
	this->emptyTiles.push_back(MakeIndex(0, 14));
	this->emptyTiles.push_back(MakeIndex(0, 15));
	this->emptyTiles.push_back(MakeIndex(1, 14));
	this->emptyTiles.push_back(MakeIndex(1, 15));
	this->emptyTiles.push_back(MakeIndex(2, 0));
	this->emptyTiles.push_back(MakeIndex(2, 1));
	this->emptyTiles.push_back(MakeIndex(2, 12));
	this->emptyTiles.push_back(MakeIndex(2, 13));
	this->emptyTiles.push_back(MakeIndex(2, 14));
	this->emptyTiles.push_back(MakeIndex(3, 12));
	this->emptyTiles.push_back(MakeIndex(3, 13));
	this->emptyTiles.push_back(MakeIndex(3, 14));
	this->emptyTiles.push_back(MakeIndex(13, 0));
	this->emptyTiles.push_back(MakeIndex(12, 1));
	this->emptyTiles.push_back(MakeIndex(20, 1));
	this->emptyTiles.push_back(MakeIndex(20, 2));
	this->emptyTiles.push_back(MakeIndex(20, 3));
	this->emptyTiles.push_back(MakeIndex(20, 4));
	this->emptyTiles.push_back(MakeIndex(21, 1));
	this->emptyTiles.push_back(MakeIndex(21, 2));*/

	//this->emptyTiles.push_back(MakeIndex(10, 4));
}

bool GridComputation::IsTileColorEmpty(Color c)
{
	//return false to disable it
	return this->emptyTileColors.In(c);
}

bool GridComputation::IsTileIndexAssumedEmpty(Index index)
{
	auto find = std::find(this->emptyTiles.begin(), this->emptyTiles.end(), index);
	
	return find == this->emptyTiles.end();
}

void GridComputation::ComputeGridBlock(GridIndex*  grid      , 
									   Index index           , 
	                                   Bitmap tileElement    , 
	                                   Bitmap gridElement    , 
	                                   const TileSet* tileSet, 
	                                   Color transColor      , 
	                                   Byte solidThreshold)
{
	for (auto i = 0; i < GRID_ELEMENTS_PER_TILE; i++)
	{
		auto x = i % GRID_BLOCK_ROWS;
		auto y = i / GRID_BLOCK_ROWS;

		al_draw_bitmap_region(gridElement, (x * GRID_ELEMENT_WIDTH), (y * GRID_ELEMENT_HEIGHT), GRID_ELEMENT_WIDTH, GRID_ELEMENT_HEIGHT, 0, 0, 0);

		auto isEmpty = this->ComputeIsGridIndexEmpty(gridElement , transColor, solidThreshold);

		*grid++ = isEmpty ? GRID_EMPTY_TILE : GRID_SOLID_TILE;
	}
}

bool GridComputation::ComputeIsGridIndexEmpty(Bitmap gridElem, Color transColor, Byte solidThreashold)
{
	auto n = 0;

	AccessBitmapPixels(gridElem, [this, transColor, &n](Color color) {
		auto diff = (color.r != transColor.r) && 
					(color.g != transColor.g) &&
					(color.b != transColor.b) &&
					(color.a != transColor.a);

		if ( diff && !this->IsTileColorEmpty(color))
			++n;
	});

	return (n <= solidThreashold);
}

void GridComputation::ComputeTileGridBlocks(const TileLayer* map, GridIndex* grid)
{
	/*for (Dim row = 0; row < map->GetMapHeight(); row++)
	{
		for (Dim col = 0; col < map->GetMapWidth(); col++)
		{
			auto isEmpty = this->IsTileIndexAssumedEmpty(map->GetTile(row, col));

			auto value = ((isEmpty) ? (GRID_EMPTY_TILE) : (GRID_SOLID_TILE));

			memset(grid, value, GRID_BLOCK_COLUMNS);
			memset(grid + GRID_WIDTH, value, GRID_BLOCK_COLUMNS);
			memset(grid + (2 * GRID_WIDTH), value, GRID_BLOCK_COLUMNS);
			memset(grid + (3 * GRID_WIDTH), value, GRID_BLOCK_COLUMNS);

			grid += GRID_BLOCK_COLUMNS;
		}
		grid += (3 * GRID_WIDTH);
	}*/

	for (auto row = 0; row < map->GetMapHeight(); ++row)
		for (auto col = 0; col < map->GetMapWidth(); ++col) 
		{
			auto isEmpty = this->IsTileIndexAssumedEmpty(map->GetTile(row, col));

			auto value = ((isEmpty) ? (GRID_EMPTY_TILE) : (GRID_SOLID_TILE));

			memset(grid, value, GRID_ELEMENTS_PER_TILE);

			grid += GRID_ELEMENTS_PER_TILE;
		}
}

void GridComputation::ComputeTileGridBlocks(const TileLayer* map    , 
											GridIndex* grid       , 
	                                        const TileSet* tileset, 
	                                        Color transColor      , 
	                                        Byte solidThreshold)
{
	Bitmap tileElement = al_create_bitmap(TILE_WIDTH, TILE_HEIGHT);
	Bitmap gridElement = al_create_bitmap(GRID_ELEMENT_WIDTH, GRID_ELEMENT_HEIGHT);

	assert(tileElement && gridElement);

	for (auto row = 0; row < map->GetMapHeight(); row++)
		for (auto col = 0; col < map->GetMapWidth(); col++)
		{
			Index index = map->GetTile(row, col);

			al_set_target_bitmap(tileElement);

			tileset->PutTile(0 , 0 , index);

			if (this->IsTileIndexAssumedEmpty(index)) 
			{
				this->emptyTileColors.Insert(tileElement, index);
				memset(grid, GRID_EMPTY_TILE, GRID_ELEMENTS_PER_TILE);
				grid += GRID_ELEMENTS_PER_TILE;
			}else
				this->ComputeGridBlock(grid, index, tileElement, gridElement, tileset, transColor, solidThreshold);
		}

	al_destroy_bitmap(tileElement);
	al_destroy_bitmap(gridElement);
}