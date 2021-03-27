#ifndef FRAME_RANGE_ANIMATOR_H
#define FRAME_RANGE_ANIMATOR_H

#include "Animator.h"
#include "FrameRangeAnimation.h"

#include <assert.h>

class FrameRangeAnimator : public Animator
{
	protected:

		FrameRangeAnimation* anim  = nullptr;

		unsigned currFrame         = 0; // animation state
		unsigned currRep           = 0; // animation state
	
    public:
		void Progress(Timestamp currTime) override;
		
		unsigned GetCurrFrame() const;
		unsigned GetCurrRep() const;

		FrameRangeAnimation* GetAnim() const;
		
		void Start(FrameRangeAnimation* _anim, Timestamp _t);

		FrameRangeAnimator() = default;

		~FrameRangeAnimator();
};
#endif // !FRAME_RANGE_ANIMATOR_H

