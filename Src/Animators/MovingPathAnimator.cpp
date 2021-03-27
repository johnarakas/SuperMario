#include "MovingPathAnimator.h"

void MovingPathAnimator::Progress(Timestamp currTime)
{
	if (this->anim->GetPath().size() == 0)
		return;

	auto end   = this->anim->GetPath().size();

	while (currTime > this->lastTime && ((currTime - this->lastTime) >= this->anim->GetPath()[this->pathPos].delay))
	{
		this->lastTime += this->anim->GetPath()[this->pathPos].delay;

		this->NotifyAction(*this->anim);

		this->pathPos++;
		if (this->pathPos == end)
		{
			this->currRep++;
			
			if (!this->anim->IsForever() && (this->currRep == this->anim->GetReps()))
			{
				this->state = ANIMATOR_FINISHED;
				this->pathPos = 0;
				this->currRep = 0;
				this->NotifyStopped();
				return;
			}
			else // restart the path
			{
				this->pathPos = 0;
			}
		}
	}
}

unsigned MovingPathAnimator::GetCurrPathPos() const
{
	return this->pathPos;
}

void MovingPathAnimator::SetCurrPathPos(unsigned _pathPos)
{
	this->pathPos = _pathPos;
}

unsigned MovingPathAnimator::GetCurrRep() const
{
	return this->currRep;
}

void MovingPathAnimator::SetCurrRep(unsigned _reps)
{
	this->currRep = _reps;
}

MovingPathAnimation* MovingPathAnimator::GetAnim()
{
	return this->anim;
}

void MovingPathAnimator::Start(MovingPathAnimation * _anim, Timestamp _t)
{
	this->anim       = _anim;
	this->lastTime   = _t;
	this->state      = ANIMATOR_RUNNING;

	this->NotifyStarted();
}

MovingPathAnimator::~MovingPathAnimator()
{
	delete anim;
	anim = nullptr;
}
