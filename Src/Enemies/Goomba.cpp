#include "Goomba.h"
#include "Mario.h"

EnemiesTempate<Goomba> goombaManager;

const CollisionAction Goomba::MakeCollisionAction()
{
	return ([this](Sprite* s1, Sprite* s2)
	{
		Mario& mario   = Mario::GetMario();
		auto marioState = mario.GetMarioState();

		if (!s1->GetVisibility())
			return;

		if (mario.GetMarioSprite() != s2)
			return;

		if (marioState == MARIO_DYING   ||
			marioState == MARIO_DEAD)
			return;

		if (ScoreLayer::GetLives() <= 0)
			return;
	
		auto& goombas = goombaManager.GetAliveEnemies();

		auto marioPosX = s2->GetPositionX();
		auto marioPosY = s2->GetPositionY();

		for (auto* goomb : goombas)
		{
			auto state = this->GetState();

			if (state != GOOMBA_WALKING)
				continue;

			auto goombaPosX = goomb->GetSprite()->GetPositionX();
			auto goombaPosY = goomb->GetSprite()->GetPositionY();

			auto collideCorrectY = (goombaPosY >= marioPosY) ? goombaPosY - marioPosY : marioPosY - goombaPosY;
			auto isThatGoombaFromUp = collideCorrectY <= 14;

			auto collideCorrectX = (goombaPosX >= marioPosX) ? goombaPosX - marioPosX : marioPosX - goombaPosX;
			auto isThatGoombaSide = collideCorrectX <= 14;
				
			if ( (marioState == MARIO_FALLING_LEFT  ||  marioState == MARIO_FALLING_RIGHT) && 
				  isThatGoombaFromUp && 
				  isThatGoombaSide)
			{
				goomb->MakeACrushedAnimation(goomb->GetSprite()->GetPositionX(), goomb->GetSprite()->GetPositionY());
				ADD_SCORE(100);
					
				return;
			}
			else
			{
				if (isThatGoombaSide && isThatGoombaFromUp)
				{
					auto marioType = mario.GetMarioType();
						
					switch (marioType)
					{
						case SMALL_INVINCIBLE_MARIO:
						case SUPER_INVINCIBLE_MARIO:
							goomb->SetState(GOOMBA_DEAD);
							ADD_SCORE(100);
							break;
						case SMALL_MARIO:
							REMOVE_LIVE(1);
								
							PLAYSOUND("mario_die");
							mario.MarioDie();
							break;
						case SUPER_MARIO:
							mario.SetMarioType(SMALL_MARIO);
							break;
					}
					return;
				}
			}
		}
	});
}

const CollisionAction Goomba::MakeCollisionActionWithItem(Goomba* goomba)
{
	return [goomba](Sprite* s1 , Sprite* s2) 
	{
		if (!s1->GetVisibility() || !s2->GetVisibility())
			return;

		if (goomba->GetState() != GOOMBA_WALKING)
			return;

		auto& bricks = Items::GetSigleton().GetAnimators();

		for(auto brick : bricks)
		{
			if (brick.first->GetTypeId() != R_BRICK_TYPE &&
				brick.first->GetTypeId() != Q_BRICK_TYPE)
				continue;

			auto brickPosX = brick.first->GetPositionX();
			auto brickPosY = brick.first->GetPositionY();

			auto goombaPosX = goomba->GetSprite()->GetPositionX();
			auto goombaPosY = goomba->GetSprite()->GetPositionY();

			auto collideCorrectY = (goombaPosY >= brickPosY) ? goombaPosY - brickPosY : brickPosY - goombaPosY;
			auto isThatGoombaFromUp = collideCorrectY <= 14 && collideCorrectY > 0;

			auto collideCorrectX = (goombaPosX >= brickPosX) ? goombaPosX - brickPosX : brickPosX - goombaPosX;
			auto isThatGoombaSide = collideCorrectX <= 14 && collideCorrectX > 0;

			if (isThatGoombaSide && isThatGoombaFromUp)
			{
				goomba->SetState(GOOMBA_DEAD);
				return;
			}
		}
	};
}

void Goomba::MakeGoomba(Dim x, Dim y)
{
	GridLayer* grid = Terrain::GetActionLayer().GetGridLayer();

	std::string type(GOOMBA_WALKING_TYPE);

	this->SetState(GOOMBA_WALKING);

	this->goombaFilm = new Sprite(GETFILM(type), x, y, type);
	this->goombaFilm->SetBoundingArea(new BoundingBox(x, y, x + TILE_WIDTH, y + TILE_HEIGHT));
	this->goombaFilm->SetMover(Sprite::MakeSpriteMover(grid, this->goombaFilm));

	Sprite::PrepareSpriteGravity(grid, this->goombaFilm);

	FrameRangeAnimation* goombaAnimation = new FrameRangeAnimation(type, 0, 1, 0, 4, 0, 60);

	this->goombaFilm->GetGravityHandler().SetOnStartFalling([this , goombaAnimation]()
	{
		if (this->GetState() == GOOMBA_CRASHED ||
			this->GetState() == GOOMBA_DEAD)
			return;

		goombaAnimation->SetDx(0);
		goombaAnimation->SetDy(4);

		this->SetState(GOOMBA_FALLING);
	});

	this->goombaFilm->GetGravityHandler().SetOnStopFalling([this, goombaAnimation]()
	{
		if (this->GetState() == GOOMBA_CRASHED ||
			this->GetState() == GOOMBA_DEAD)
			return;

		goombaAnimation->SetDx(4);
		goombaAnimation->SetDy(0);

		this->SetState(this->oldState);
	});

	this->goombaAnimator = new FrameRangeAnimator();

	this->goombaAnimator->SetOnAction([this , grid](Animator* _animator, const Animation& _animation)
	{
		auto* frameAnimator = (FrameRangeAnimator*)_animator;
		auto& frameAnimation = (FrameRangeAnimation&)_animation;

		//change the powerUp's direction
		int x = frameAnimation.GetDx();
		int y = 0;

		grid->FilterGridMotion(this->goombaFilm->GetFrameBox(), &x, &y);

		if (x == 0 && this->GetState() != GOOMBA_FALLING)
			frameAnimation.SetDx(-frameAnimation.GetDx());

		if ((frameAnimator->GetCurrFrame() != frameAnimation.GetStartFrame()) ||
			frameAnimator->GetCurrRep())
		{
			//change the frame and move the character
			this->goombaFilm->Move(frameAnimation.GetDx(), frameAnimation.GetDy());
		}

		this->goombaFilm->SetFrame(frameAnimator->GetCurrFrame());
		this->goombaFilm->SetBoundingAreaFromFrameBox();
	});

	for (auto* marioSprite : Mario::GetMario().GetMarioSprites())
		REGISTER(this->goombaFilm, marioSprite, this->MakeCollisionAction());
	
	for (auto brick : Items::GetSigleton().GetAnimators())
	{
		if (brick.first->GetTypeId() == Q_BRICK_TYPE || brick.first->GetTypeId() == R_BRICK_TYPE)
			REGISTER(this->goombaFilm, brick.first, this->MakeCollisionActionWithItem(this));
	}

	this->goombaAnimator->Start(goombaAnimation, GETTIME);
}

Goomba::Goomba(Dim x, Dim y)
{	
	this->MakeGoomba(x, y);

	goombaManager.Register(this);
}

GoombaState Goomba::GetState()
{
	return this->goombaState;
}

void Goomba::SetState(GoombaState _state)
{
	this->oldState	  = this->goombaState;
	this->goombaState = _state;
}

Sprite* Goomba::GetSprite()
{
	return this->goombaFilm;
}

FrameRangeAnimator* Goomba::GetAnimator()
{
	return this->goombaAnimator;
}

void Goomba::MakeACrushedAnimation(Dim x, Dim y)
{
	std::string type(GOOMBA_CRUSHED_TYPE);

	Sprite* goombaCrashedFilm = new Sprite(GETFILM(type), x, y + 8, type);
	goombaCrashedFilm->SetBoundingArea(new BoundingBox(x, y + 8, x + TILE_WIDTH, y + TILE_HEIGHT + 8));
	goombaCrashedFilm->SetHasDirectMotion(true);

	this->goombaFilm->SetVisibility(false);
	this->SetState(GOOMBA_CRASHED);

	FrameRangeAnimation* goombaAnimation = new FrameRangeAnimation(type, 0, 2, 1, 0, 0, 800);

	this->goombaAnimator->SetOnAction([goombaCrashedFilm](Animator* _animator , const Animation& _animation) 
	{
		auto* frameAnimator  = (FrameRangeAnimator*)_animator;
		auto& frameAnimation = (FrameRangeAnimation&)_animation;

		goombaCrashedFilm->SetFrame(frameAnimator->GetCurrFrame());
	});

	this->goombaAnimator->SetOnFinish([this , goombaCrashedFilm](Animator* _animator)
	{
		this->SetState(GOOMBA_DEAD);

		SPRITE_MARK_AS_DEAD(goombaCrashedFilm);
	});

	this->goombaAnimator->Start(goombaAnimation, GETTIME);
}

bool Goomba::IsDead() const
{
	return this->goombaState == GOOMBA_DEAD;
}
