#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Animation.h"

#include <functional>

using Timestamp = uint64_t;

enum AnimatorState 
{
	ANIMATOR_FINISHED = 0, 
	ANIMATOR_RUNNING  = 1, 
	ANIMATOR_STOPPED  = 2
};

class Animator
{
	public:

		using OnFinish = std::function<void(Animator*)>;
		using OnStart  = std::function<void(Animator*)>;
		using OnAction = std::function<void(Animator*, const Animation&)>;

	protected:

		Timestamp		lastTime = 0;
		AnimatorState   state	 = ANIMATOR_FINISHED;
		
		OnFinish onFinish;
		OnStart  onStart;
		OnAction onAction;
	
	protected:
		
		void NotifyStopped(void);
		void NotifyStarted(void);
		void NotifyAction(const Animation&);
		
		void Finish(bool isForced = false);

	public:

		void Stop(void);
		bool HasFinished(void) const;
		
		void TimeShift(Timestamp offset);

		void SetLastTime(Timestamp _lastTime);
		Timestamp& GetLastTime();
		
		virtual void Progress(Timestamp currTime) = 0;
		
		template<typename Tfunc>void SetOnFinish(const Tfunc& f);
		template<typename Tfunc>void SetOnStart(const Tfunc& f);
		template<typename Tfunc>void SetOnAction(const Tfunc& f);

		Animator();
		Animator(const Animator&) = delete;
		Animator(Animator&&)      = delete;

		virtual ~Animator();
};


template<typename Tfunc>
inline void Animator::SetOnFinish(const Tfunc& _onFinish)
{
	this->onFinish = _onFinish;
}

template<typename Tfunc>
inline void Animator::SetOnStart(const Tfunc& _onStart)
{
	this->onStart = _onStart;
}

template<typename Tfunc>
inline void Animator::SetOnAction(const Tfunc& _onAction)
{
	this->onAction = _onAction;
}

#endif // !ANIMATOR_H
