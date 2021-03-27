#ifndef ENEMIES_TEMPLATE_H
#define ENEMIES_TEMPLATE_H

#include "SpriteManager.h"
#include "AnimatorManager.h"
#include "Terrain.h"

#include <vector>

template<class T> class EnemiesTempate 
{	
	private:

		std::vector<T*> aliveEnemies;
		std::vector<T*> deadEnemies;

	private:

		void EnableUsingViewWindow();
		void DestroyUsingViewWindow();
		void CheckForDead();

	public:

		EnemiesTempate() = default;
		~EnemiesTempate() = default;

		void Register(T* _enemie);
		void Remove(T* _enemie);

		void MarkAsDead(T* _enemie);
		void CommitDestruction();

		void Actions();

		std::vector<T*>& GetAliveEnemies();
		std::vector<T*>& GetDeadEnemies();
};

template<class T>
inline void EnemiesTempate<T>::EnableUsingViewWindow()
{
	auto scene = Terrain::GetActionLayer().GetCurrentScene();
	auto scenes = Terrain::GetActionLayer().GetScene();

	for (typename std::vector<T*>::iterator enemie = aliveEnemies.begin(); enemie != aliveEnemies.end(); enemie++)
	{
		if ((*enemie)->GetSprite()->GetPositionX() < scenes[scene].tileX * TILE_WIDTH)
		{
			(*enemie)->GetSprite()->SetVisibility(false);
			SUSPEND((*enemie)->GetAnimator());
		}
		else
		{
			(*enemie)->GetSprite()->SetVisibility(true);
			RUN((*enemie)->GetAnimator());
		}
	}
}

template<class T>
inline void EnemiesTempate<T>::DestroyUsingViewWindow()
{
	Rect viewWindow = Terrain::GetActionLayer().GetViewWindow();

	for (typename std::vector<T*>::iterator enemie = aliveEnemies.begin(); enemie != aliveEnemies.end();)
	{
		if ((*enemie)->GetSprite()->GetPositionY() >= 14 * TILE_HEIGHT)
		{
			this->deadEnemies.push_back((*enemie));

			enemie = this->aliveEnemies.erase(enemie);
		}
		else
			enemie++;
	}
}

template<class T>
inline void EnemiesTempate<T>::CheckForDead()
{
	for (typename std::vector<T*>::iterator enemie = aliveEnemies.begin(); enemie != aliveEnemies.end();)
		if ((*enemie)->IsDead())
		{
			this->deadEnemies.push_back((*enemie));

			enemie = this->aliveEnemies.erase(enemie);
		}
		else
			enemie++;
}

template<class T>
inline void EnemiesTempate<T>::Register(T* _enemie)
{
	assert(_enemie);

	aliveEnemies.push_back(_enemie);
}


template<class T>
inline void EnemiesTempate<T>::Remove(T* _enemie)
{
	assert(_enemie);

	for (typename std::vector<T*>::iterator enemie = aliveEnemies.begin(); enemie != aliveEnemies.end(); enemie++)
		if ((*enemie) == _enemie)
		{
			aliveEnemies.erase(enemie);
			return;
		}
}

template<class T>
inline void EnemiesTempate<T>::MarkAsDead(T* _enemie)
{
	assert(_enemie);

	Remove(_enemie);

	deadEnemies.push_back(_enemie);
}

template<class T>
inline void EnemiesTempate<T>::CommitDestruction()
{
	for (typename std::vector<T*>::iterator dead = deadEnemies.begin(); dead != deadEnemies.end(); dead++)
	{
		SPRITE_MARK_AS_DEAD((*dead)->GetSprite());
		ANIMATOR_MARK_AS_DEAD((*dead)->GetAnimator());

		delete (*dead);
		(*dead) = nullptr;
	}
	deadEnemies.clear();
}

template<class T>
inline void EnemiesTempate<T>::Actions()
{
	DestroyUsingViewWindow();
	
	CheckForDead();

	if (Terrain::GetActionLayer().GetDisplayChanged())
		EnableUsingViewWindow();
}

template<class T>
inline std::vector<T*>& EnemiesTempate<T>::GetAliveEnemies()
{
	return aliveEnemies;
}

template<class T>
inline std::vector<T*>& EnemiesTempate<T>::GetDeadEnemies()
{
	return deadEnemies;
}

#endif // !ENEMIES_TEMPLATE_H
