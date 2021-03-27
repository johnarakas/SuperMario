#ifndef BITMAP_LOADER_H
#define BITMAP_LOADER_H

#include "Utilities.h"

#include <string>
#include <map>
#include <iostream>

using Bitmaps = std::map<std::string, Bitmap>;

class BitmapLoader 
{
	private:
		
		Bitmaps bitmaps;

		static BitmapLoader loader;

	private:
		
		BitmapLoader() = default;

		Bitmap GetBitmap(std::string& path);


	public:

		void Destroy();

		static BitmapLoader& GetSigleton();

		Bitmap LoadBitmap(const char* path);
};

#endif // !BITMAP_LOADER_H
