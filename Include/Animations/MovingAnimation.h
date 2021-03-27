#ifndef MOVING_ANIMATION_H
#define MOVING_ANIMATION_H

#include "Animation.h"

class MovingAnimation : public Animation
{
	protected:

		unsigned reps  = 1; // 0=forever
		unsigned delay = 0;

		int dx = 0;
		int dy = 0;

	public:

		int GetDx() const;
		MovingAnimation& SetDx(int _dx);

		int GetDy() const;
		MovingAnimation& SetDy(int _dy);

		unsigned GetDelay() const;
		MovingAnimation& SetDelay(unsigned _delay);

		unsigned GetReps() const;
		MovingAnimation& SetReps(unsigned _reps);

		bool IsForever() const;
		MovingAnimation& SetForever();

		Animation* Clone() const override;

		MovingAnimation(const std::string& _id,
						unsigned _reps        ,
						int _dx               ,
						int _dy               ,
						unsigned _delay);
};

#endif // !MOVING_ANIMATION_H

