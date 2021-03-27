#include "FrameRangeAnimator.h"

void FrameRangeAnimator::Progress(Timestamp currTime)
{
	while (currTime > this->lastTime && (currTime - this->lastTime) >= this->anim->GetDelay()) 
	{
		if (this->currFrame == this->anim->GetEndFrame()) 
		{
			this->currFrame = this->anim->GetStartFrame(); // flip to start
		}
		else
			++this->currFrame;

		this->lastTime += this->anim->GetDelay();
		this->NotifyAction(*this->anim);

		if (this->currFrame == this->anim->GetEndFrame())
		{
			this->currRep++;
			if (!this->anim->IsForever() && (this->currRep == this->anim->GetReps()))
			{
				this->state   = ANIMATOR_FINISHED;
				this->currRep = 0;
				this->NotifyStopped();
				return;
			}
		}
	}
}

unsigned FrameRangeAnimator::GetCurrFrame() const
{ 
	return this->currFrame; 
}

unsigned FrameRangeAnimator::GetCurrRep() const
{ 
	return this->currRep; 
}

FrameRangeAnimation* FrameRangeAnimator::GetAnim() const
{
	return this->anim;
}

void FrameRangeAnimator::Start(FrameRangeAnimation* _anim, Timestamp _t)
{
	this->anim		= _anim;
	this->lastTime	= _t;
	this->state		= ANIMATOR_RUNNING;
	this->currFrame = this->anim->GetStartFrame();
	this->currRep	= 0;
	
	this->NotifyStarted();
	this->NotifyAction(*this->anim);
}

FrameRangeAnimator::~FrameRangeAnimator()
{
	delete anim;
	anim = nullptr;
}
