#ifndef PIRANHA_H
#define PIRANHA_H

#include "AnimationFilmHolder.h"
#include "AnimatorManager.h"
#include "MovingPathAnimator.h"
#include "Sprite.h"
#include "SystemTime.h"
#include "SpriteManager.h"
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

#define PIRANHA_TYPE		"piranhaplant.biting"
#define PIRANHA_X_OFFSET	TILE_WIDTH/2
#define PIRANHA_Y_OFFSET	0
#define PIRANHA_Y_PLUS		7

enum PiranhaState
{
	PIRANHA_DEAD,
	PIRANHA_DOWN,
	PIRANHA_UP,
	PIRANHA_ALIVE
};

class Piranha
{
	private:

		Sprite* piranhaFilm = nullptr;

		MovingPathAnimator* piranhaAnimator = nullptr;

		PiranhaState piranhaState = PIRANHA_DOWN;

		const CollisionAction MakeCollisionAction();

	private:

		void CreatePiranha(Dim x, Dim y);

	public:

		Piranha() = delete;
		Piranha(Dim x, Dim y);

		Sprite* GetSprite();
		MovingPathAnimator* GetAnimator();

		PiranhaState GetState();
		void SetState(PiranhaState _state);

		bool IsDead() const;
};

extern EnemiesTempate<Piranha> piranhasManager;

#endif // !PIRANHA_H

