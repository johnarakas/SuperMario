#include "TickAnimator.h"

void TickAnimator::Progress(Timestamp currTime)
{
	if (!this->anim->IsDiscrete()) 
	{
		this->elapsedTime = currTime - this->lastTime;
		this->lastTime    = currTime;
		
		this->NotifyAction(*this->anim);
	}
	else
	{
		while (currTime > this->lastTime && (currTime - this->lastTime) >= this->anim->GetDelay()) 
		{
			this->lastTime += this->anim->GetDelay();
			
			this->NotifyAction(*this->anim);

			if (!this->anim->IsForever() && ++this->currRep == this->anim->GetReps()) 
			{
				this->state			= ANIMATOR_FINISHED;
				this->currRep		= 0;
				this->elapsedTime	= 0;
				this->NotifyStopped();
				return;
			}
		}
	}
}

unsigned TickAnimator::GetCurrRep(void) const
{
	return this->currRep;
}

Timestamp TickAnimator::GetElapsedTime(void) const
{
	return this->elapsedTime;
}

float TickAnimator::GetElapsedTimeNormalised(void) const
{
	return float(elapsedTime) / float(anim->GetDelay());
}

void TickAnimator::Start(const TickAnimation& _anim, Timestamp _t)
{
	this->anim			= (TickAnimation*)_anim.Clone();
	this->lastTime		= _t;
	this->state			= ANIMATOR_RUNNING;
	this->currRep		= 0;
	this->elapsedTime	= 0;
	this->NotifyStarted();
}
