#ifndef COLLISION_CHECKER_H
#define COLLISION_CHECKER_H

#include "Sprite.h"

#include <functional>
#include <tuple>
#include <list>

using CollisionAction	= std::function<void(Sprite* s1, Sprite* s2)>;
using Entry				= std::tuple<Sprite*, Sprite*, CollisionAction>;

class CollisionChecker final 
{
	private:

		static CollisionChecker checker;
	
	protected:

		std::list<Entry> entries;

	public:

		template<typename Tfunc> void Register(Sprite* s1, Sprite* s2, const Tfunc& _action);

		void Cancel(Sprite* s1, Sprite* s2);

		void Cancel(Sprite* s1);

		void Check() const;

		static CollisionChecker& GetSigleton();
		static const CollisionChecker& GetSigletonConst();
};

template<typename Tfunc>
inline void CollisionChecker::Register(Sprite* s1, Sprite* s2, const Tfunc& _action)
{
	this->entries.push_back(std::make_tuple(s1, s2, _action));
}

#define REGISTER(item , mario , action) (CollisionChecker::GetSigleton().Register(item , mario , action))
#define CANCEL(item , mario)			(CollisionChecker::GetSigleton().Cancel(item , mario))
#define _CANCEL(item)					(CollisionChecker::GetSigleton().Cancel(item))

#endif // !COLLISION_CHECKER_H
