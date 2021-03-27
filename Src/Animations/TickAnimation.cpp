#include "TickAnimation.h"

bool TickAnimation::Inv(void) const
{
	return this->isDiscrete || this->reps == 1;
}

unsigned TickAnimation::GetDelay() const
{
	return this->delay;
}

TickAnimation& TickAnimation::SetDelay(unsigned _delay)
{
	this->delay = _delay;
	return *this;
}

unsigned TickAnimation::GetReps() const
{
	return this->reps;
}

TickAnimation& TickAnimation::SetReps(unsigned _reps)
{
	this->reps = _reps;
	return *this;
}

bool TickAnimation::IsForever() const
{
	return !reps;
}

TickAnimation& TickAnimation::SetForever()
{
	reps = 0;
	return *this;
}

bool TickAnimation::IsDiscrete() const
{
	return isDiscrete;
}

Animation* TickAnimation::Clone() const
{
	return new TickAnimation(this->id, this->delay, this->reps, this->isDiscrete);
}

TickAnimation::TickAnimation(const std::string& _id, unsigned _delay, unsigned _reps, bool _discrete) : Animation(_id)
{
	this->delay		 = _delay;
	this->reps		 = _reps;
	this->isDiscrete = _discrete;

	assert(this->Inv());
}