#include "CollisionChecker.h"

CollisionChecker CollisionChecker::checker;

void CollisionChecker::Cancel(Sprite* s1, Sprite* s2)
{
	this->entries.remove_if([s1 , s2](const Entry& e)
	{
		auto is = (std::get<0>(e) == s1 && std::get<1>(e) == s2) ||
				  (std::get<0>(e) == s2 && std::get<1>(e) == s1);
		return is;
	});
}

void CollisionChecker::Cancel(Sprite* s1)
{
	this->entries.remove_if([s1](const Entry& e) 
	{
		auto is = (std::get<0>(e) == s1 || std::get<1>(e) == s1);

		return is;
	});
}

void CollisionChecker::Check() const
{
	for (auto& entry : this->entries)
	{
		auto collide = std::get<0>(entry)->CollisionCheck(std::get<1>(entry));
		
		if (collide)
			std::get<2>(entry)(std::get<0>(entry), std::get<1>(entry));
	}
}

CollisionChecker& CollisionChecker::GetSigleton()
{
	return checker;
}

const CollisionChecker& CollisionChecker::GetSigletonConst()
{
	return checker;
}
