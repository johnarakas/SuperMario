#include "Animator.h"
#include "AnimatorManager.h"

void Animator::NotifyStopped(void)
{
	AnimatorManager::GetSigleton().MarkAsSuspended(this);
	if (this->onFinish)
		(this->onFinish)(this);
}

void Animator::NotifyStarted(void)
{
	AnimatorManager::GetSigleton().MarkAsRunning(this);
	if (this->onStart)
		(this->onStart)(this);
}

void Animator::NotifyAction(const Animation& animation)
{
	if (this->onAction)
		(this->onAction)(this, animation);
}

void Animator::Finish(bool isForced)
{
	if (!this->HasFinished())
	{
		this->state = isForced ? ANIMATOR_STOPPED : ANIMATOR_FINISHED;
		this->NotifyStopped();
	}
}

void Animator::Stop(void)
{
	this->Finish(true);
}

bool Animator::HasFinished(void) const
{ 
	return state != ANIMATOR_RUNNING; 
}

void Animator::TimeShift(Timestamp offset)
{
	this->lastTime += offset;
}

void Animator::SetLastTime(Timestamp _lastTime)
{
	this->lastTime = _lastTime;
}

Timestamp& Animator::GetLastTime()
{
	return this->lastTime;
}

Animator::Animator()
{
	AnimatorManager::GetSigleton().Register(this);
}

Animator::~Animator()
{
	//AnimatorManager::GetSigleton().Cancel(this);
}
