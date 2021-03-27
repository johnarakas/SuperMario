#ifndef FRAME_RANGE_ANIMATION_H
#define FRAME_RANGE_ANIMATION_H

#include "MovingAnimation.h"

class FrameRangeAnimation : public MovingAnimation
{
	protected:

		unsigned start = 0;
		unsigned end   = 0;
	
	public:

		unsigned GetStartFrame() const;
		FrameRangeAnimation& SetStartFrame(unsigned _start);
		
		unsigned GetEndFrame() const;
		FrameRangeAnimation& SetEndFrame(unsigned _end);
		
		Animation* Clone() const override;
		
		FrameRangeAnimation(const std::string& _id,
							unsigned _start       , 
							unsigned _end         ,
							unsigned _reps        ,  
							int		 _dx          , 
							int      _dy          , 
							int      _delay);
};

#endif // !FRAME_RANGE_ANIMATION_H

