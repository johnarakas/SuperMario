#include "MovingAnimation.h"

int MovingAnimation::GetDx() const
{
	return this->dx;
}

MovingAnimation& MovingAnimation::SetDx(int _dx)
{ 
	this->dx = _dx; 
	return *this; 
}

int MovingAnimation::GetDy() const 
{ 
	return this->dy; 
}

MovingAnimation& MovingAnimation::SetDy(int _dy)
{ 
	this->dy = _dy; 
	return *this; 
}

unsigned MovingAnimation::GetDelay() const 
{ 
	return this->delay; 
}

MovingAnimation& MovingAnimation::SetDelay(unsigned _delay)
{ 
	this->delay = _delay; 
	return *this; 
}

unsigned MovingAnimation::GetReps() const 
{ 
	return this->reps; 
}

MovingAnimation& MovingAnimation::SetReps(unsigned _reps)
{ 
	this->reps = _reps; 
	return *this; 
}

bool MovingAnimation::IsForever() const 
{ 
	return !this->reps; 
}

MovingAnimation& MovingAnimation::SetForever()
{ 
	this->reps = 0; 
	return *this; 
}

Animation* MovingAnimation::Clone() const
{
	return new MovingAnimation(this->id, this->reps, this->dx, this->dy, this->delay);
}

MovingAnimation::MovingAnimation(const std::string& _id, 
								 unsigned _reps        , 
								 int _dx               , 
								 int _dy               , 
								 unsigned _delay)      : Animation(_id) 
{
	this->dx	= _dx;
	this->dy	= _dy;
	this->reps	= _reps;
	this->delay = _delay;
}
