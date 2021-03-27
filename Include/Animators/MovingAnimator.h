#ifndef MOVING_ANIMATOR_H
#define MOVING_ANIMATOR_H

#include "MovingAnimation.h"
#include "Animator.h"

class MovingAnimator : public Animator
{
	protected:

		MovingAnimation* anim = nullptr;
		
		unsigned currRep      = 0; // animation state
	
	public:

		void Progress(Timestamp currTime) override;
		
		MovingAnimation* GetAnim();
		
		void Start(MovingAnimation* _anim, Timestamp _t);

		MovingAnimator() = default;

		~MovingAnimator();
};

#endif // !MOVING_ANIMATOR_H
