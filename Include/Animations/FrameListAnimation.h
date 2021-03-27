#ifndef FRAME_LIST_ANIMATION_H
#define FRAME_LIST_ANIMATION_H

#include "MovingAnimation.h"

#include <vector>

using Frames = std::vector<unsigned int>;

class FrameListAnimation : public MovingAnimation
{
	protected:

		Frames frames;
	
	public:
		
		const Frames& GetFrames() const;
		void SetFrames(const Frames& _frames);

		Animation* Clone() const override;

		FrameListAnimation(const std::string& _id   ,
						   const Frames&     _frames,
						   unsigned _reps           ,
						   int      _dx             , 
						   int      _dy             , 
						   unsigned _delay
		);
};

#endif // !FRAME_LIST_ANIMATION_H

