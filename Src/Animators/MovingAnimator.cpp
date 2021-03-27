#include "MovingAnimator.h"

void MovingAnimator::Progress(Timestamp currTime)
{
	while (currTime > this->lastTime && ((currTime - this->lastTime) >= this->anim->GetDelay())) 
	{
		this->lastTime += this->anim->GetDelay();
		
		this->NotifyAction(*this->anim);
		
		this->currRep++;
		if (!this->anim->IsForever() && (this->currRep == this->anim->GetReps())) 
		{
			this->state   = ANIMATOR_FINISHED;
			this->currRep = 0;
			this->NotifyStopped();
		}
	}
}

MovingAnimation* MovingAnimator::GetAnim()
{
	return this->anim;
}

void MovingAnimator::Start(MovingAnimation* _anim, Timestamp _t)
{
	this->anim     = _anim;
	this->lastTime = _t;
	this->state    = ANIMATOR_RUNNING;
	this->currRep  = 0;

	this->NotifyStarted();
}

MovingAnimator::~MovingAnimator()
{
	delete anim;
	anim = nullptr;
}
