#include "MotionQuantizer.h"

MotionQuantizer& MotionQuantizer::SetRange(int _horiz, int _verti)
{
	this->horizMax = _horiz;
	this->vertiMax = _verti;

	this->used	   = true;

	return *this;
}

void MotionQuantizer::Move(const Rect & r, int* dx, int* dy)
{
	if (!this->used)
		this->mover(r, dx, dy);
	else
		do
		{
			auto dxFinal = std::min(number_sign(*dx) * this->horizMax, *dx);
			auto dyFinal = std::min(number_sign(*dy) * this->vertiMax, *dy);
			
			this->mover(r, &dxFinal, &dyFinal);
			
			// X motion denied
			if (!dxFinal)
				*dx = 0;
			else
				*dx -= dxFinal;
			
			// Y motion denied
			if (!dyFinal)
				*dy = 0;
			else
				*dy -= dyFinal;
		
		} while (*dx || *dy);
}
