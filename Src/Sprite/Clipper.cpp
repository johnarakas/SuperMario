#include "Clipper.h"

bool Clipper::ClipRect(const Rect& r, const Rect& area, Rect* result) const
{
	return clip_rect(
						r.x,
						r.y,
						r.w,
						r.h,
						area.x,
						area.y,
						area.w,
						area.h,
						&result->x,
						&result->y,
						&result->w,
						&result->h
					);
}

bool Clipper::Clip(const Rect& r, const Rect& displayArea, Point* displayPos, Rect* clippedBox) const
{
	Rect visibleArea;
	
	if (!this->ClipRect(r, this->view(), &visibleArea))
	{
		clippedBox->w = 0;
		clippedBox->h = 0;

		return false;
	}
	else 
	{
		clippedBox->x = r.x - visibleArea.x;
		clippedBox->y = r.y - visibleArea.y;
		clippedBox->w = visibleArea.w;
		clippedBox->h = visibleArea.h;

		displayPos->x = displayArea.x + (visibleArea.x - view().x);
		displayPos->y = displayArea.y + (visibleArea.y - view().y);
		
		return true;
	}
}
