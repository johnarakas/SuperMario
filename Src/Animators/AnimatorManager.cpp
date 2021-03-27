#include "AnimatorManager.h"

AnimatorManager AnimatorManager::manager;

void AnimatorManager::CleanUp()
{
	//Clear all running animators
	for (auto anim : this->running)
	{
		delete anim;
		anim = nullptr;
	}
	this->running.clear();

	//Clear all suspended animators
	for (auto anim : this->suspended)
	{
		delete anim;
		anim = nullptr;
	}
	this->suspended.clear();

	//Clear all dead animators
	for (auto anim : this->dead)
	{
		delete anim;
		anim = nullptr;
	}
	this->dead.clear();

	//Clear all paused animators
	for (auto anim : this->paused)
	{
		delete anim;
		anim = nullptr;
	}
	this->paused.clear();
}

void AnimatorManager::Register(Animator* animator)
{
	//assert(animator->HasFinished());

	this->suspended.push_back(animator);
}

void AnimatorManager::Cancel(Animator* animator)
{
	auto suspended_exist = std::find(this->suspended.begin(), this->suspended.end(), animator);
	auto running_exist	 = std::find(this->running.begin(), this->running.end(), animator);

	auto s_exist = (suspended_exist != this->suspended.end());
	auto r_exist = (running_exist != this->running.end());

	if (s_exist)
		this->suspended.erase(suspended_exist);
	if (r_exist)
		this->running.erase(running_exist);
}

void AnimatorManager::MarkAsRunning(Animator* animator)
{
	auto exist = std::find(this->suspended.begin(), this->suspended.end(), animator);

	if (exist == this->suspended.end())
		return;

	this->suspended.erase(exist);

	animator->TimeShift(GETTIME - animator->GetLastTime());
	this->running.push_back(animator);
}

void AnimatorManager::MarkAsSuspended(Animator* animator)
{
	auto exist = std::find(this->running.begin(), this->running.end(), animator);
	
	if (exist == this->running.end())
		return;

	this->running.erase(exist);
	this->suspended.push_back(animator);
}

void AnimatorManager::MarkAsDead(Animator * animator)
{
	this->Cancel(animator);

	this->dead.push_back(animator);
}

void AnimatorManager::CommitDestruction()
{
	for (auto* anim : this->dead) 
	{
		delete anim;
		anim = nullptr;
	}
	this->dead.clear();
}

void AnimatorManager::Progress(Timestamp currTime) 
{
	auto copied(this->running);
	for (auto* animator : copied)
		animator->Progress(currTime);
}

void AnimatorManager::TimeShift(Timestamp dt)
{
	for (auto* a : this->running)
		a->TimeShift(dt);
}

void AnimatorManager::Pause()
{
	this->paused.insert(this->paused.begin(), this->running.begin(), this->running.end());
	this->running.clear();
}

void AnimatorManager::Resume()
{
	this->running.insert(this->running.begin(), this->paused.begin(), this->paused.end());
	this->paused.clear();
}

AnimatorManager& AnimatorManager::GetSigleton()
{
	return manager;
}

const AnimatorManager& AnimatorManager::GetSigletonConst()
{
	return manager;
}

AnimatorManager::~AnimatorManager()
{
	this->CleanUp();
}
