#ifndef CLIPPER_H
#define CLIPPER_H

#include "Shapes.h"

#include <functional>
#include <algorithm>

using View = std::function<const Rect&(void)>;

class Clipper 
{
	private:

		View view;

	private:

		bool ClipRect(const Rect& r, const Rect& area, Rect* result) const;
	
	public:
		
		template<typename Tfunc> Clipper& SetView(const Tfunc& _view);
		
		bool Clip(const Rect& r,const Rect& displayArea, Point* displayPos, Rect* clippedBox) const;
		
		Clipper(void)			= default;
		Clipper(const Clipper&) = default;

};

template <class T> bool clip_rect(T x,   T y,   T w,   T h,
								  T wx,  T wy,  T ww,  T wh,
								  T* cx, T* cy, T* cw, T* ch
)
{
	*cw = T(std::min(wx + ww, x + w)) - (*cx = T(std::max(wx, x)));
	*ch = T(std::min(wy + wh, y + h)) - (*cy = T(std::max(wy, y)));
	return *cw > 0 && *ch > 0;
}

template<typename Tfunc>
inline Clipper& Clipper::SetView(const Tfunc & _view)
{
	this->view = _view;

	return *this;
}

#endif // !CLIPPER_H
