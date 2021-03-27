#include "Game.h"

ALLEGRO_DISPLAY*			  display;
ALLEGRO_EVENT_QUEUE*		  queue;
ALLEGRO_TIMER*				  timer;
ALLEGRO_EVENT				  event;
ALLEGRO_KEYBOARD_STATE		  state;

void app::Game::SetIsFinished(bool _isFinished)
{
	this->isFinished = _isFinished;
}

bool app::Game::GetIsFinished() const
{
	return this->isFinished;
}

void app::Game::Pause(uint64_t t)
{
	isPaused  = true;

	pauseTime = t;

	Invoke(pauseResume);
}

void app::Game::Resume()
{
	isPaused = false;

	Invoke(pauseResume);
	
	pauseTime = 0;
}

bool app::Game::IsPaused() const
{
	return isPaused;
}

uint64_t app::Game::GetPauseTime() const
{
	return pauseTime;
}

void app::Game::MainLoop(void)
{
	while (!IsFinished())
		MainLoopIteration();
}

void app::Game::MainLoopIteration(void) 
{
	SystemTime::SetCurrentTime();

	Render();
	Input();
	if (!IsPaused())
	{
		ProgressAnimations();
		AI();
		Physics();
		CollisionChecking();
		CommitDestructions();
		UserCode();
	}
}