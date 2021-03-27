#ifndef MARIO_H
#define MARIO_H

#include "AnimationFilmHolder.h"
#include "AnimatorManager.h"
#include "CollisionChecker.h"
#include "FrameRangeAnimator.h"
#include "Items.h"
#include "MovingAnimator.h"
#include "MovingPathAnimator.h"
#include "ScoreLayer.h"
#include "SystemTime.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Terrain.h"
#include "Piranha.h"

#define SETSTATE(state)    (SetMarioState(state))
#define ISSTATE(state)     (GetMarioState() == state)

using MarioSprites = std::vector<Sprite*>;

enum MarioState
{
	MARIO_STANDING_LEFT,
	MARIO_STANDING_RIGHT,
	MARIO_WALKING_LEFT,
	MARIO_WALKING_RIGHT,
	MARIO_JUMPING_RIGHT,
	MARIO_JUMPING_LEFT,
	MARIO_RUNNING_RIGHT,
	MARIO_RUNNING_LEFT,
	MARIO_FALLING_LEFT,
	MARIO_FALLING_RIGHT,
	MARIO_DYING,
	MARIO_ENTER_PIPE,
	MARIO_DEAD,
};

enum MarioType 
{
	SMALL_MARIO,
	SUPER_MARIO,
	SMALL_INVINCIBLE_MARIO,
	SUPER_INVINCIBLE_MARIO
};

class Mario
{
	private:

		GridLayer* gridLayer;

		static Mario mario;

		MarioState marioState;
		MarioState oldState;
		MarioType  marioType;
		MarioType  oldType;

		Sprite* marioFallingRightFilm	= nullptr;
		Sprite* marioFallingLeftFilm	= nullptr;
		Sprite* marioStandingRightFilm	= nullptr;
		Sprite* marioStandingLeftFilm	= nullptr;
		Sprite* marioWalkingRightFilm	= nullptr;
		Sprite* marioWalkingLeftFilm	= nullptr;
		Sprite* marioJumpingRightFilm   = nullptr;
		Sprite* marioJumpingLeftFilm	= nullptr;
		Sprite* marioDyingFilm 			= nullptr;
		Sprite* marioEnterPipe			= nullptr;

		MovingAnimator* marioFallingRight		= nullptr;
		MovingAnimator* marioFallingLeft		= nullptr;
		MovingAnimator* marioStandingRight		= nullptr;
		MovingAnimator* marioStandingLeft		= nullptr;
		FrameRangeAnimator* marioWalkingRight	= nullptr;
		FrameRangeAnimator* marioWalkingLeft	= nullptr;
		MovingPathAnimator* marioJumpingRight	= nullptr;
		MovingPathAnimator* marioJumpingLeft	= nullptr;
		MovingPathAnimator* marioDying 			= nullptr;
		MovingAnimator* marioEnterPipeAnimator	= nullptr;

		MarioSprites marioFilms;

		bool isMarioOnPipe		= false;

		std::vector<Dim> checkpoints = { 0 , 23 * TILE_WIDTH};
		int currentCheckpoint		 = 0;

	private:

		void PrepareMarioSpriteGravityHandler(GridLayer* layer, Sprite* sprite);

		void InitializeMarioRightFalling();
		void InitializeMarioLeftFalling();
		void InitializeMarioStandingRight();
		void InitializeMarioStandingLeft();
		void InitializeMarioWalkingRight();
		void InitializeMarioWalkingLeft();
		void InitializeMarioJumpingRight();
		void InitializeMarioJumpingLeft();

		void MarioInitializeCollision();
		const CollisionAction MakePowerUpAction(MovingAnimator* _anim);
		void InitializeMarioRBrickCollision();
		void InitializeMarioQBrickCollision();
		void InitializeMarioCoinCollision();
		void InitializeMarioPipeCollision();

		void InitializeMarioSprites();

		void CheckForCheckPoint();

		Mario();
		~Mario() = default;

	public:

		bool onAir = false;
	
	public:

		void MakeInvisibleMarioTimer();

		void SetMarioState(MarioState _state);
		MarioState& GetMarioState();

		void SetMarioType(MarioType _type);
		MarioType& GetMarioType();	

		bool IsDead()				const;
		bool IsSuperMario()			const;
		bool IsDying()				const;
		bool IsMarioOnPipe()		const;

		bool CheckMarioOnSecondPipe();

		void MakeMarioDie();

		Sprite* GetMarioSprite();
		MarioSprites& GetMarioSprites();
		MarioSprites& GetMarioWalkingSprites();
		MarioSprites& GetMarioFallingSprites();
		MarioSprites& GetMarioJumpingSprites();
		MarioSprites& GetMarioStandingSprites();

		void SetFilmsPosition(Dim _x, Dim _y);
		void SetFilmsPositionX(Dim _x);
		void SetFilmsPositionY(Dim _y);
		void AddFilmsPositionX(Dim _dx);
		void SetFilmsBoundingArea();
		
		void MarioWalkRight();
		void MarioWalkLeft();
		void MarioRunRight();
		void MarioRunLeft();
		void MarioStandingAndJumping();

		void MarioDie();

		void MarioEnterPipe();
		void MarioExitPipe();

		void Draw(Bitmap at);

		void InitializeMario();

		static Mario& GetMario();
};

#endif // !MARIO_H

