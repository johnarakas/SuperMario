#include "FlashAnimation.h"

unsigned FlashAnimation::GetRepetitions() const
{
	return this->repetitions;
}

FlashAnimation& FlashAnimation::SetRepetitions(unsigned _repetitions)
{
	this->repetitions = _repetitions;
	return *this;
}

unsigned FlashAnimation::GetShowDeay() const
{
	return this->showDelay;
}

FlashAnimation& FlashAnimation::SetShowDeay(unsigned _showDelay)
{
	this->showDelay = _showDelay;
	return *this;
}

unsigned FlashAnimation::GetHideDeay() const
{
	return this->hideDelay;
}

FlashAnimation& FlashAnimation::SetHideDeay(unsigned _hideDelay)
{
	this->hideDelay = _hideDelay;
	return *this;
}

Animation* FlashAnimation::Clone() const
{
	return new FlashAnimation(this->id, this->repetitions, this->hideDelay, this->showDelay);
}

FlashAnimation::FlashAnimation(const std::string& _id,
							   unsigned _repetitions,
							   unsigned _showDelay,
							   unsigned _hideDelay) : Animation(_id)
{
	this->repetitions = _repetitions;
	this->showDelay   = _showDelay;
	this->hideDelay   = _hideDelay;
}
