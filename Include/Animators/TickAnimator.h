#ifndef TICK_ANIMATOR_H
#define TICK_ANIMATOR_H

#include "Animator.h"
#include "TickAnimation.h"

class TickAnimator : public Animator
{
	protected:
		
		TickAnimation* anim		= nullptr;
		
		unsigned currRep		= 0;
		Timestamp elapsedTime	= 0;
	
	public:

		void Progress(Timestamp currTime) override;

		unsigned GetCurrRep(void) const;
		Timestamp GetElapsedTime(void) const;
		
		float GetElapsedTimeNormalised(void) const;

		void Start(const TickAnimation& _anim, Timestamp _t);

		TickAnimator(void) = default;
};

#endif // !TICK_ANIMATOR_H
