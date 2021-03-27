#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>

class Animation 
{
	protected:
		
		std::string id;
	
	public:

		const std::string& GetId(void)		{ return this->id; };
		
		void SetId(const std::string& _id)	{ this->id = _id; };
		
		virtual Animation* Clone(void)		const = 0;
		
		Animation(const std::string& _id)	{ this->id = _id; };
		
		virtual ~Animation()				{};
};

#endif // !ANIMATION_H

