#include "Terrain.h"

Terrain Terrain::terrain;

TileLayer Terrain::actionLayer = TileLayer(TILE_MAP_HEIGHT, TILE_MAP_WIDTH, new TileSet());

CircularBackground* Terrain::background = nullptr;

GameOverLayer Terrain::gameOver;

StartLayer Terrain::startLayer;

uint64_t Terrain::lastTime = 0;

void Terrain::CreateSigleton()
{
	//Load here all terrain's data
	actionLayer.GetTileSet()->LoadTileSet(TILE_SET_PATH);
	actionLayer.ReadTextMap(ACTION_LAYER_PATH);
	actionLayer.ReadScenes(SCENES_PATH);

	gameOver.ItializeGameOverLayer(GAME_OVER_PATH);

	startLayer.ItializeStartGameLayer(START_GAME_PATH);

	GridLayer* gridLayer = new GridLayer(GRID_HEIGHT, GRID_WIDTH);

	GridComputation* gridComputation = new GridComputation();

	gridComputation->ComputeTileGridBlocks(&actionLayer, gridLayer->GetBuffer());

	actionLayer.SetGridLayer(gridLayer);

	background = new CircularBackground(BACKGROUND_PATH);

	lastTime = GETTIME;
}

TileLayer& Terrain::GetActionLayer()
{
	return actionLayer;
}

CircularBackground* Terrain::GetBackground() 
{
	return background;
}

void Terrain::Display(Bitmap dest, bool isPaused , bool isGameOver, bool isStart)
{
	if (!isPaused)
	{
		//Now the background scrolls every 20msec 
		if (GETTIME - lastTime > 20)
		{
			background->Scroll();
			lastTime = GETTIME;
		}
	}

	if (!isGameOver)
	{
		if (!isStart)
		{
			if (!actionLayer.GetCurrentScene())
				background->Display(dest, 0, 0);
			actionLayer.Display(dest, actionLayer.GetViewWindow());
		}
		else 
		{
			startLayer.Display(dest);
		}
	}
	else
		gameOver.Display(dest);
}
