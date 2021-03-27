#ifndef TICK_ANIMATION_H
#define TICK_ANIMATION_H

#include <assert.h>

#include "Animation.h"

class TickAnimation : public Animation
{
	protected:
		
		unsigned delay      = 0;
		unsigned reps       = 1;
		bool     isDiscrete = true;
		
	protected:
		
		bool Inv(void) const;

	public:

		unsigned GetDelay() const;
		TickAnimation& SetDelay(unsigned _delay);
		
		unsigned GetReps() const;
		TickAnimation& SetReps(unsigned _reps);
		
		bool IsForever() const;
		TickAnimation& SetForever();
		bool IsDiscrete() const;
		
		Animation* Clone() const override;

		TickAnimation(const std::string& _id, unsigned _delay, unsigned _reps, bool _discrete);
};

#endif // !TICK_ANIMATION_H

