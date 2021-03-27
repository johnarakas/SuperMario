#include "MovingPathAnimation.h"

const Path& MovingPathAnimation::GetPath() const
{
	return this->path;
}

void MovingPathAnimation::SetPath(const Path& _path)
{
	this->path = _path;
}

bool MovingPathAnimation::IsForever()
{
	return !this->reps;
}

void MovingPathAnimation::SetForever()
{
	this->reps = 0;
}

void MovingPathAnimation::SetReps(unsigned _reps)
{
	this->reps = _reps;
}

unsigned MovingPathAnimation::GetReps() const
{
	return this->reps;
}

Animation* MovingPathAnimation::Clone() const
{
	return new MovingPathAnimation(this->id, this->path);
}

MovingPathAnimation::MovingPathAnimation(const std::string& _id, const Path& _path) : Animation(_id)
{
	this->path = _path;
}

MovingPathAnimation::MovingPathAnimation(const std::string & _id, const Path & _path, unsigned _reps) : Animation(_id)
{
	path = _path;
	reps = _reps;
}
