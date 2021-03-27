#ifndef KOOPA_TROOPA_H
#define KOOPA_TROOPA_H

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
#include "TickAnimator.h"
#include "EnemiesTemplate.h"

#define KOOPA_WALKING_RIGHT		"koopatroopa.walking_right"
#define KOOPA_WALKING_LEFT		"koopatroopa.walking_left"
#define KOOPA_DEAD				"koopatroopa.dying"
#define KOOPA_STUNNED			"koopatroopa.crashed"
#define KOOPA_STUNNED_MOVING	"koopatroopa.crashed_moving"

#define STUNNED_DELAY	6000
#define KOOPA_OFFSET	6
#define KOOPA_STUNNED_X 2
#define KOOPA_STUNNED_Y 4

enum KoopaTroopaState
{
	KOOPATROOPA_WALKING_LEFT,
	KOOPATROOPA_WALKING_RIGHT,
	KOOPATROOPA_STUNNED,
	KOOPATROOPA_STUNNED_MOVING,
	KOOPATROOPA_FALLING,
	KOOPATROOPA_DEAD,
};

enum KoopaTroopaType 
{
	RED,
	GREEN
};

class KoopaTroopa
{
	private:
		
		Sprite* koopaFilm = nullptr;

		FrameRangeAnimator* koopaAnimator = nullptr;

		KoopaTroopaState koopaState = KOOPATROOPA_WALKING_RIGHT;
		KoopaTroopaState prevState;

		KoopaTroopaType  koopaType;

	private:

		const CollisionAction MakeCollisionAction();

		const CollisionAction MakeCollisionActionWithItem(KoopaTroopa* koopa);

		const CollisionAction MakeCollisionActionWithGoomba();

		const CollisionAction MakeCollisionActionWithKoopaTroopa();

	public:
		void InitializeKoopaTroopa(KoopaTroopaType type, Dim x, Dim y);

		void MakeKoopaTroopaWalkingRight(KoopaTroopaType _type, Dim x, Dim y);
		void MakeKoopaTroopaWalkingLeft(KoopaTroopaType _type, Dim x, Dim y);
		void MakeKoopaTroopaStunned(KoopaTroopaType _type, Dim x, Dim y);
		void MakeKoopaTroopaStunnedMoving(KoopaTroopaType _type, Dim x, Dim y);

		KoopaTroopa() = delete;
		KoopaTroopa(KoopaTroopaType type, Dim x, Dim y);
		~KoopaTroopa() = default;

		KoopaTroopaState GetState();
		void SetState(KoopaTroopaState _state);
		
		KoopaTroopaType GetType();
		void SetType(KoopaTroopaType _type);

		Sprite* GetSprite();
		FrameRangeAnimator* GetAnimator();

		bool IsDead() const;
};

extern EnemiesTempate<KoopaTroopa> koopaTroopaManager;

#endif // !KOOPA_TROOPA_H
