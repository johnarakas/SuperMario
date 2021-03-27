#include "BitmapLoader.h"

BitmapLoader BitmapLoader::loader;

Bitmap BitmapLoader::GetBitmap(std::string& path)
{
	auto bitmap = this->bitmaps.find(path);

	return ((bitmap != this->bitmaps.end()) ? bitmap->second : nullptr);
}

void BitmapLoader::Destroy()
{
	for (auto& bitmap : this->bitmaps)
		al_destroy_bitmap(bitmap.second);
	this->bitmaps.clear();
}

BitmapLoader& BitmapLoader::GetSigleton()
{
	return loader;
}

Bitmap BitmapLoader::LoadBitmap(const char* path)
{
	std::string s_path(path);
	
	auto exist = this->GetBitmap(s_path);

	if (!exist)
	{
		exist = al_load_bitmap(path);

		this->bitmaps[s_path] = exist;
	}

	return exist;
}
