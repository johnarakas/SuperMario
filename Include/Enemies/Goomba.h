#ifndef GOOMBA_H
#define GOOMBA_H

#include "AnimationFilmHolder.h"
#include "AnimatorManager.h"
#include "CollisionChecker.h"
#include "FrameRangeAnimator.h"
#include "Items.h"
#include "MovingAnimator.h"
#include "MovingPathAnimator.h"
#include "SystemTime.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Terrain.h"
#include "EnemiesTemplate.h"

#define GOOMBA_WALKING_TYPE	"goomba.walking"
#define GOOMBA_DEAD_TYPE	"goomba.dying"
#define GOOMBA_CRUSHED_TYPE	"goomba.crashed"

enum GoombaState
{
	GOOMBA_WALKING,
	GOOMBA_DEAD,
	GOOMBA_CRASHED,
	GOOMBA_FALLING
};

class Goomba
{
	private:

		GoombaState goombaState;
		GoombaState oldState;

		Sprite* goombaFilm	= nullptr;

		FrameRangeAnimator* goombaAnimator	= nullptr;

	private:

		void MakeGoomba(Dim x, Dim y);
	
		const CollisionAction MakeCollisionAction();

		const CollisionAction MakeCollisionActionWithItem(Goomba* goomba);

	public:
		
		Goomba() = delete;
		Goomba(Dim x, Dim y);
		~Goomba() = default;

		GoombaState GetState();
		void SetState(GoombaState _state);

		Sprite* GetSprite();
		FrameRangeAnimator* GetAnimator();

		void MakeACrushedAnimation(Dim x , Dim y);

		bool IsDead() const;
};

extern EnemiesTempate<Goomba> goombaManager;

#endif