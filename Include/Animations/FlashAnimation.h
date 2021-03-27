#ifndef FLASH_ANIMATION_H
#define FLASH_ANIMATION_H

#include "Animation.h"

class FlashAnimation : public Animation
{
	private:
		
		unsigned repetitions = 0;
		unsigned hideDelay   = 0;
		unsigned showDelay   = 0;	
	
	public:

		unsigned GetRepetitions() const;
		FlashAnimation& SetRepetitions(unsigned _repetitions);
		
		unsigned GetShowDeay() const;
		FlashAnimation& SetShowDeay(unsigned _showDelay);

		unsigned GetHideDeay() const;
		FlashAnimation& SetHideDeay(unsigned _hideDelay);
		
		Animation* Clone() const override;
		
		FlashAnimation(const std::string& _id,
					   unsigned _repetitions,
					   unsigned _showDelay,
					   unsigned _hideDelay);
};

#endif // !FLASH_ANIMATION_H

