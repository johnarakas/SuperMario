#include "TileLayer.h"

TileLayer::TileLayer(Dim rows, Dim cols, TileSet* tileSet)
{
	this->totalRows = rows;
	this->totalCols = cols;
	this->tileSet   = tileSet;

	this->Allocate();
};

TileLayer::~TileLayer()
{
	//Note: this line cause a heap error
	//delete this->map;
	delete this->grid;
	delete this->tileSet;
};

void TileLayer::Allocate()
{
	this->map = new Index[this->totalRows * this->totalCols];

	memset(this->map, '\0', this->totalRows * this->totalCols);
}

void TileLayer::SetViewWindow(Rect viewWindow)
{
	this->displayChanged	= true;
	this->viewWindow		= viewWindow;
}

Rect& TileLayer::GetViewWindow()
{
	return this->viewWindow;
}

void TileLayer::SetScene(Scenes scene)
{
	this->scenes = scene;
}

Scenes& TileLayer::GetScene()
{
	return this->scenes;
}

void TileLayer::SetDisplayChanged(bool changed) 
{
	this->displayChanged = changed;
}

bool TileLayer::GetDisplayChanged() const
{
	return this->displayChanged;
}

void TileLayer::SetCurrentScene(int current)
{
	assert(current == 0 || current == 1);

	this->currentScene = current;
}

int TileLayer::GetCurrentScene() const
{
	return this->currentScene;
}

void TileLayer::SetTile(Dim row, Dim col, Index tileIndex)
{
	this->map[(row * totalCols) + col] = tileIndex;
}

Index TileLayer::GetTile(Dim row, Dim col) const
{
	return this->map[(row * totalCols) + col];
}

Index* TileLayer::GetMap() const
{
	return this->map;
}

void TileLayer::SetGridLayer(GridLayer * _grid)
{
	this->grid = _grid;
}

GridLayer* TileLayer::GetGridLayer() const
{
	return this->grid;
}

void TileLayer::SetTileSet(TileSet* tileSet)
{
	assert(tileSet);

	this->tileSet = tileSet;
}

TileSet* TileLayer::GetTileSet()
{
	return this->tileSet;
}

int TileLayer::GetMapPixelWidth() const
{
	return this->totalCols * TILE_WIDTH;
}

int TileLayer::GetMapPixelHeight() const
{
	return this->totalRows * TILE_HEIGHT;
}

Dim TileLayer::GetMapWidth() const
{
	return this->totalCols;
}

Dim TileLayer::GetMapHeight() const
{
	return this->totalRows;
}

void TileLayer::Display(Bitmap dest, Rect& viewWindow)
{
	assert(dest == al_get_target_bitmap());

	auto startCol = DIV_TILE_WIDTH(viewWindow.x);
	auto startRow = DIV_TILE_HEIGHT(viewWindow.y);
	auto endCol = DIV_TILE_WIDTH(viewWindow.x + viewWindow.w - 1);
	auto endRow = DIV_TILE_HEIGHT(viewWindow.y + viewWindow.h - 1);

	for (Dim row = startRow; row <= endRow; ++row)
		for (Dim col = startCol; col <= endCol; ++col)
			this->tileSet->PutTile(MUL_TILE_WIDTH(col - startCol), MUL_TILE_WIDTH(row - startRow), this->GetTile(row, col));
}

bool TileLayer::ReadTextMap(const char* path)
{
	assert(path);

	std::ifstream fileToRead;
	std::string line;

	Dim row = 0;
	Dim col = 0;

	Dim tileSetWidth = this->tileSet->GetTileSetWidth();

	fileToRead.open(path, std::ios::in);
	if (!fileToRead.is_open())
	{
		std::cerr << "Failed to open the tile map file for reading\n";
		return false;
	}

	while (std::getline(fileToRead, line))
	{
		std::istringstream	lineStream(line);
		std::string			cell;

		col = 0;
		while (std::getline(lineStream, cell , ','))
		{
			Dim index = static_cast<Dim>(std::stoi(cell));
			if (index == USHRT_MAX)
				this->map[(row * this->totalCols) + col] = index;
			else
				this->map[(row * this->totalCols) + col] = MakeIndex(index / tileSetWidth , index % tileSetWidth);
			col++;
		}
		row++;
	}

	fileToRead.close();

	return true;
}

bool TileLayer::WriteTextMap(const char* path)
{
	assert(path);

	std::ofstream fileToWrite;
	
	fileToWrite.open(path, std::ios::out);
	if (!fileToWrite.is_open())
	{
		std::cerr << "Failed to open the tile map file for writing\n";
		return false;
	}

	for (Dim row = 0; row < this->totalRows; row++)
		for (Dim col = 0; col < this->totalCols; col++)
		{
			fileToWrite << this->GetTile(row, col);

			if (col == (this->totalCols - 1))
				fileToWrite << "\n";
			else
				fileToWrite << ",";
		}

	fileToWrite.close();
		
	return true;
}

bool TileLayer::ReadScenes(const char* path)
{
	assert(path);

	std::ifstream fileToRead;
	std::string line;

	fileToRead.open(path, std::ios::in);
	if (!fileToRead.is_open())
	{
		std::cerr << "Failed to open the tile map file for reading\n";
		return false;
	}

	int scenesNo = 0;

	fileToRead >> scenesNo;

	for (auto scene = 0; scene < scenesNo; scene++)
	{
		Dim tileX;
		Dim tileY;

		Dim marioPosX;
		Dim marioPosY;

		Dim viewWindowWidth;
		Dim viewWindowHeight;

		fileToRead >> tileX;
		fileToRead >> tileY;

		fileToRead >> marioPosX;
		fileToRead >> marioPosY;

		fileToRead >> viewWindowWidth;
		fileToRead >> viewWindowHeight;

		this->scenes[scene] = Scene{ tileX, tileY, marioPosX, marioPosY, viewWindowWidth, viewWindowHeight };
	}

	fileToRead.close();

	return true;
}

void TileLayer::FilterScrollDistance(int viewStartCoord, int viewSize, int* d, int maxMapSize)
{
	if (*d == 0)
		return;

	auto val = *d + viewStartCoord;

	if (val < 0)
		*d = viewStartCoord;
	else
		if ((val + viewSize) >= maxMapSize)
			*d = maxMapSize -  (viewStartCoord + viewSize);
}


void TileLayer::FilterScroll(int* dx, int* dy) {
	this->FilterScrollDistance(
		this->viewWindow.x, this->viewWindow.w, dx, this->GetMapPixelWidth()
	);
	this->FilterScrollDistance(
		this->viewWindow.y, this->viewWindow.h, dy, this->GetMapPixelHeight()
	);
}

void TileLayer::Scroll(int dx, int dy)
{
	this->viewWindow.x += dx;
	this->viewWindow.y += dy;
}

void TileLayer::ScrollWithBoundsCheck(int dx, int dy ) 
{
	this->FilterScroll(&dx, &dy);
	this->Scroll(dx, dy);
}

bool TileLayer::CanScrollHoriz(int dx)
{
	return (this->viewWindow.x >= -dx) && ((this->viewWindow.x + this->viewWindow.w + dx) <= this->GetMapPixelWidth());
}

bool TileLayer::CanScrollVert(int dy)
{
	return (this->viewWindow.y >= -dy) && ((this->viewWindow.y + this->viewWindow.h + dy) <= this->GetMapPixelHeight());
}

Point TileLayer::PickTile(Dim x, Dim y)
{
	Point point;

	point.x = DIV_TILE_WIDTH(x + this->viewWindow.x);
	point.y = DIV_TILE_HEIGHT(y + this->viewWindow.y);

	return point;
}
