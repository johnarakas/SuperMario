#include "ScrollAnimation.h"

const Scroll& ScrollAnimation::GetScroll() const
{
	return this->scroll;
}

void ScrollAnimation::SetScroll(const Scroll& _scroll)
{
	this->scroll = _scroll;
}

Animation* ScrollAnimation::Clone() const
{
	return new ScrollAnimation(this->id, this->scroll);
}

ScrollAnimation::ScrollAnimation(const std::string& _id, const Scroll& _scroll) : Animation(_id)
{
	this->scroll = _scroll;
}
