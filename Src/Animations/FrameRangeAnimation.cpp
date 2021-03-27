#include "FrameRangeAnimation.h"

unsigned FrameRangeAnimation::GetStartFrame() const
{ 
	return this->start; 
}

FrameRangeAnimation& FrameRangeAnimation::SetStartFrame(unsigned _start)
{ 
	this->start = _start; 
	return *this; 
}

unsigned FrameRangeAnimation::GetEndFrame() const
{ 
	return this->end; 
}

FrameRangeAnimation& FrameRangeAnimation::SetEndFrame(unsigned _end)
{ 
	this->end = _end; 
	return *this; 
}

Animation* FrameRangeAnimation::Clone() const
{
	return new FrameRangeAnimation(this->id       , 
								   this->start    , 
								   this->end      , 
								   this->GetReps(), 
								   this->GetDx()  , 
								   this->GetDy()  , 
								   this->GetDelay());
}

FrameRangeAnimation::FrameRangeAnimation(const std::string& _id,
										 unsigned _start       , 
										 unsigned _end         ,
										 unsigned _reps        , 
										 int      _dx          , 
										 int      _dy          , 
										 int      _delay
) : MovingAnimation(_id, _reps, _dx, _dy, _delay) 
{
	this->start = _start;
	this->end   = _end;
}