#include "StartLayer.h"

void StartLayer::ItializeStartGameLayer(std::string path)
{
	this->layer = BitmapLoader::GetSigleton().LoadBitmap(path.c_str());

	if (!layer)
	{
		std::cerr << "Error : failed to load the Game Over Layer.\n";
		return;
	}
}

void StartLayer::Display(Bitmap at)
{
	al_draw_bitmap_region(this->layer, 0, 0, MAX_SCREEN_WIDTH, MAX_SCREEN_HEIGHT, 0, 0, 0);
}
