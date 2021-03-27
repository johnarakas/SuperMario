#include "CircularBackground.h"

void CircularBackground::Scroll()
{
	this->viewWin.x += this->scroll;

	if (this->viewWin.x < 0)
		this->viewWin.x = al_get_bitmap_width(this->bg) + this->viewWin.x;
	else
		if (this->viewWin.x >= al_get_bitmap_width(this->bg))
			this->viewWin.x = this->viewWin.x - al_get_bitmap_width(this->bg);

}

void CircularBackground::SetScroll(int _dx)
{
	this->scroll = _dx;
}

int CircularBackground::GetScroll() const
{
	return this->scroll;
}

void CircularBackground::Display(Bitmap dest, int x, int y) const
{
	auto bg_w = al_get_bitmap_width(bg);
	auto w1   = std::min(bg_w - viewWin.x, static_cast<int>(viewWin.w));

	//al_set_target_bitmap(dest);
	al_draw_bitmap_region(bg, viewWin.x, viewWin.y, w1, viewWin.h, x, y, 0);

	if (w1 < viewWin.w)
	{
		auto w2 = viewWin.w - w1;
		al_draw_bitmap_region(bg, 0, viewWin.y, w2, viewWin.h, x + w1, y, 0);
	}
}

Bitmap CircularBackground::GetBackGroundBitmap() const
{
	return this->bg;
}


CircularBackground::CircularBackground(const char* bgPath)
{
	assert(bgPath);

	this->bg = BitmapLoader::GetSigleton().LoadBitmap(bgPath);
}
