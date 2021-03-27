#include "FrameListAnimation.h"

const Frames& FrameListAnimation::GetFrames() const
{
	return this->frames;
}

void FrameListAnimation::SetFrames(const Frames& _frames)
{
	this->frames = _frames;
}

Animation* FrameListAnimation::Clone() const
{
	return new FrameListAnimation(this->id       ,
								  this->frames   ,
								  this->GetReps(),
								  this->GetDx()  ,
								  this->GetDy()  ,
								  this->GetDelay()
	);
}

FrameListAnimation::FrameListAnimation(const std::string& _id	,
									   const Frames&     _frames,
									   unsigned _reps			,
									   int      _dx				,
									   int      _dy				,
									   unsigned _delay
) : MovingAnimation(_id, _reps, _dx, _dy, _delay)
{
	this->frames = _frames;
}
