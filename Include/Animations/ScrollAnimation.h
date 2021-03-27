#ifndef SCROLL_ANIMATION_H
#define SCROLL_ANIMATION_H

#include "Animation.h"

#include <vector>

struct ScrollEntry
{
	int dx			= 0;
	int dy			= 0;
	unsigned delay	= 0;
};

using Scroll = std::vector<ScrollEntry>;

class ScrollAnimation: public Animation
{
	private:
		
		Scroll scroll;
	
	public:

		const Scroll& GetScroll() const;
		
		void SetScroll(const Scroll& _scroll);
		
		Animation* Clone() const override;
		
		ScrollAnimation(const std::string& _id, const Scroll& _scroll);
};

#endif // !SCROLL_ANIMATION_H

