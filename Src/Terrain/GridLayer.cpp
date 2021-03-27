#include "GridLayer.h"

void GridLayer::Allocate()
{
	this->grid = new GridIndex[this->total];

	memset(this->grid, GRID_EMPTY_TILE, this->total);

	assert( this->grid );
}

void GridLayer::FilterGridMotionDown(const Rect& r, int* dy)
{
	auto currentPosition = r.y + r.h - 1;
	auto nextPositionY   = currentPosition + *dy;

	if (nextPositionY >= MAX_PIXEL_HEIGHT)
		*dy = MAX_PIXEL_HEIGHT - currentPosition;
	else
	{
		auto newRow = DIV_GRID_ELEMENT_HEIGHT(nextPositionY);
		auto currRow = DIV_GRID_ELEMENT_HEIGHT(currentPosition);

		//We have actually moved down!
		if (newRow != currRow)
		{
			assert((newRow - 1) == currRow);

			auto startCol = DIV_GRID_ELEMENT_WIDTH(r.x);
			auto endCol   = DIV_GRID_ELEMENT_WIDTH(r.x + r.w - 1);

			for (auto col = startCol; col <= endCol; ++col)
				if (!this->CanPassGridTile(newRow, col, GRID_TOP_SOLID_MASK))
				{
					*dy = MUL_GRID_ELEMENT_HEIGHT(newRow) - (currentPosition + 1);
					break;
				}
		}
	}
}

void GridLayer::FilterGridMotionUp(const Rect& r, int* dy)
{
	auto nextPositionY = r.y + *dy;

	if (nextPositionY < 0)
		*dy = -r.y;
	else
	{
		auto newRow	 = DIV_GRID_ELEMENT_HEIGHT(nextPositionY);
		auto currRow = DIV_GRID_ELEMENT_HEIGHT(r.y);

		//We have actually moved up!
		if (newRow != currRow)
		{
			assert((newRow + 1) == currRow);

			auto startCol = DIV_GRID_ELEMENT_WIDTH(r.x);
			auto endCol   = DIV_GRID_ELEMENT_WIDTH(r.x + r.w - 1);

			for (auto col = startCol; col <= endCol; ++col)
				if (!this->CanPassGridTile(newRow, col, GRID_BOTTOM_SOLID_MASK))
				{
					*dy = MUL_GRID_ELEMENT_HEIGHT(currRow) - r.y;
					break;
				}
		}
	}
}

void GridLayer::FilterGridMotionLeft(const Rect& r, int* dx)
{
	auto nextPositionX = r.x + *dx;

	if (nextPositionX < 0)
		*dx = -r.x;
	else 
	{
		auto newColumn	= DIV_GRID_ELEMENT_WIDTH(nextPositionX);
		auto currColumn	= DIV_GRID_ELEMENT_WIDTH(r.x);

		//We have actually moved to the left!
		if (newColumn != currColumn) 
		{
			assert(	(newColumn + 1) == currColumn );

			auto startRow	= DIV_GRID_ELEMENT_HEIGHT(r.y);
			auto endRow		= DIV_GRID_ELEMENT_HEIGHT(r.y + r.h - 1);

			for (auto row = startRow; row <= endRow; ++row)
				if (!this->CanPassGridTile(row, newColumn, GRID_RIGHT_SOLID_MASK))
				{
					*dx = MUL_GRID_ELEMENT_WIDTH(currColumn) - r.x;
					break;
				}
		}
	}
}

void GridLayer::FilterGridMotionRight(const Rect& r, int* dx)
{
	auto currentPosition = r.x + r.w - 1;
	auto nextPositionX   = currentPosition + *dx;

	if (nextPositionX >= MAX_PIXEL_WIDTH)
		*dx = MAX_PIXEL_WIDTH - currentPosition;
	else 
	{
		auto newColumn  = DIV_GRID_ELEMENT_WIDTH(nextPositionX);
		auto currColumn = DIV_GRID_ELEMENT_WIDTH(currentPosition);
		
		//We have actually moved to the right!
		if (newColumn != currColumn)
		{
			assert(	(newColumn - 1) == currColumn );

			auto startRow	= DIV_GRID_ELEMENT_HEIGHT(r.y);
			auto endRow		= DIV_GRID_ELEMENT_HEIGHT(r.y + r.h - 1);

			for (auto row = startRow; row <= endRow; ++row)
				if (!this->CanPassGridTile(row, newColumn, GRID_LEFT_SOLID_MASK))
				{
					*dx = MUL_GRID_ELEMENT_WIDTH(newColumn) - (currentPosition + 1);
					break;
				}
		}
	}
}

void GridLayer::SetGridTileFlags(Dim row, Dim col, GridIndex flags)
{
	this->SetGridIndex(row, col, flags);
}

void GridLayer::SetSolidGridTile(Dim row, Dim col)
{
	this->SetGridIndex(row, col, GRID_SOLID_TILE);
}

void GridLayer::SetEmptyGridTile(Dim row, Dim col)
{
	this->SetGridIndex(row, col, GRID_EMPTY_TILE);
}

void GridLayer::SetGridTileTopSolidOnly(Dim row, Dim col, GridIndex flags)
{
	this->SetGridIndex(row, col, GRID_TOP_SOLID_MASK);
}

bool GridLayer::CanPassGridTile(Dim row, Dim col, GridIndex flags)
{
	return ( (this->GetGridIndex(row , col) & (flags)) == 0 );
}

GridLayer::GridLayer(unsigned rows, unsigned cols)
{
	this->totalRows		= rows;
	this->totalColumns	= cols;
	this->total			= rows * cols;

	this->Allocate();
}

void GridLayer::FilterGridMotion(const Rect& r, int* dx, int* dy)
{
	assert(	abs(*dx) <= GRID_ELEMENT_WIDTH && 
			abs(*dy) <= GRID_ELEMENT_HEIGHT );

	if (*dx < 0)
		this->FilterGridMotionLeft(r, dx);
	else if(*dx > 0)
		this->FilterGridMotionRight(r, dx);

	if (*dy < 0)
		this->FilterGridMotionUp(r, dy);
	else if (*dy > 0)
		this->FilterGridMotionDown(r, dy);
}

bool GridLayer::IsOnSolidGround(const Rect& r)
{
	int dy = 1;
	
	this->FilterGridMotionDown(r, &dy);

	return dy == 0;
}

GridIndex GridLayer::GetGridIndex(Dim row, Dim col)
{
	auto _row = ((row / GRID_ELEMENT_HEIGHT) * GRID_ELEMENTS_PER_TILE * TILE_MAP_WIDTH) +
				((col / GRID_ELEMENT_WIDTH) * GRID_ELEMENTS_PER_TILE) +
				((row % GRID_ELEMENT_HEIGHT) * GRID_ELEMENT_WIDTH) + (col % GRID_ELEMENT_WIDTH);
	//return this->grid[(row * this->totalColumns) + col];
	return this->grid[_row];
}

void GridLayer::SetGridIndex(Dim row, Dim col, GridIndex index)
{
	assert(	index >= 0 && index < 256 );

	this->grid[(row * this->totalColumns) + col] = index;
}

GridIndex* GridLayer::GetGridTileBlock(Dim rowTile, Dim colTile)
{
	return this->grid + (((rowTile * TILE_MAP_WIDTH) + colTile) * GRID_BLOCK_SIZEOF);
}

void GridLayer::SetGridTileBlock(Dim rowTile, Dim colTile, GridIndex flags)
{
	memset( this->GetGridTileBlock(rowTile, colTile) , flags , GRID_BLOCK_SIZEOF);
}

void GridLayer::SetDisplayGrid(bool _displayGrid)
{
	this->displayGrid = _displayGrid;
}

bool GridLayer::GetDisplayGrid() const
{
	return this->displayGrid;
}

GridIndex* GridLayer::GetBuffer()
{
	return this->grid;
}

Point GridLayer::PickGridIndex(const Rect& viewWindow, Dim x, Dim y)
{
	Point gridIndex;

	gridIndex.x = DIV_GRID_ELEMENT_WIDTH(x + viewWindow.x);
	gridIndex.y = DIV_GRID_ELEMENT_HEIGHT(y + viewWindow.y);

	return gridIndex;
}
