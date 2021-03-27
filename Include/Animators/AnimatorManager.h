#ifndef ANIMATOR_MANAGER_H
#define ANIMATOR_MANAGER_H

#include "Animator.h"
#include "SystemTime.h"

#include <assert.h>
#include <vector>

using Queue = std::vector<Animator*>;

class AnimatorManager
{
	private:
		Queue running;
		Queue suspended;

		Queue paused;

		Queue dead;
		
		static AnimatorManager manager;
		
	private:

		void CleanUp();

		AnimatorManager()						= default;
		AnimatorManager(const AnimatorManager&) = delete;
		AnimatorManager(AnimatorManager&&)      = delete;

		~AnimatorManager();
	
	public:

		void Register(Animator* animator);

		void Cancel(Animator* animator);
		
		void MarkAsRunning(Animator* animator);

		void MarkAsSuspended(Animator* animator);

		void MarkAsDead(Animator* animator);

		void CommitDestruction();
		
		void Progress(Timestamp currTime);

		void TimeShift(Timestamp dt);

		void Pause();

		void Resume();

		static AnimatorManager& GetSigleton();
		static const AnimatorManager& GetSigletonConst();
};

#define SUSPEND(animator)				(AnimatorManager::GetSigleton().MarkAsSuspended(animator))
#define RUN(animator)					(AnimatorManager::GetSigleton().MarkAsRunning(animator))
#define ANIMATOR_MARK_AS_DEAD(animator) (AnimatorManager::GetSigleton().MarkAsDead(animator))

#endif // !ANIMATOR_MANAGER_H

