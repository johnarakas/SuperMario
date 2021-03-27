#ifndef MOVING_PATH_ANIMATOR
#define MOVING_PATH_ANIMATOR

#include "Animator.h"
#include "MovingPathAnimation.h"

class MovingPathAnimator : public Animator
{
	private:

		MovingPathAnimation* anim = nullptr;

		unsigned pathPos   = 0;
		unsigned currRep   = 0;

	public:
		void Progress(Timestamp currTime) override;

		unsigned GetCurrPathPos() const;
		void SetCurrPathPos(unsigned _pathPos);

		unsigned GetCurrRep() const;
		void SetCurrRep(unsigned _reps);

		MovingPathAnimation* GetAnim();

		void Start(MovingPathAnimation* _anim, Timestamp _t);

		MovingPathAnimator() = default;

		~MovingPathAnimator();
};

#endif // !MOVING_PATH_ANIMATOR
