#include "KoopaTroopa.h"
#include "Mario.h"

EnemiesTempate<KoopaTroopa> koopaTroopaManager;

std::vector<std::string> koopaTypes = { "red" , "green" };

bool youCanHitHim = false;

const CollisionAction KoopaTroopa::MakeCollisionAction()
{
	return ([this](Sprite* s1, Sprite* s2)
	{
		auto& mario = Mario::GetMario();
		
		if (!s1->GetVisibility())
			return;

		if (mario.GetMarioSprite() != s2)
			return;

		auto marioState = mario.GetMarioState();

		if (marioState == MARIO_DYING ||
			marioState == MARIO_DEAD)
			return;

		if (ScoreLayer::GetLives() <= 0)
			return;

		auto& koopas = koopaTroopaManager.GetAliveEnemies();

		auto marioPosX = s2->GetPositionX();
		auto marioPosY = s2->GetPositionY();
		auto marioType = mario.GetMarioType();

		auto marioDead = [&mario](auto _collide) -> bool
		{
			if (_collide)
			{
				switch (mario.GetMarioType()) 
				{
					case SUPER_MARIO:
						mario.SetMarioType(SMALL_MARIO);
						return true;
					case SMALL_MARIO:
						REMOVE_LIVE(1);

						PLAYSOUND("mario_die");
						mario.MarioDie();
						return true;
				}
			}
			return false;
		};

		auto remove = [](KoopaTroopa* _koopa) 
		{
			SPRITE_MARK_AS_DEAD(_koopa->GetSprite());
			ANIMATOR_MARK_AS_DEAD(_koopa->GetAnimator());
		};

		for (auto* koopa : koopas)
		{
			auto state = koopa->GetState();

			if (state == KOOPATROOPA_DEAD)
				continue;

			auto koopaPosX = koopa->GetSprite()->GetPositionX();
			auto koopaPosY = koopa->GetSprite()->GetPositionY();

			auto collideCorrectX = (koopaPosX >= marioPosX) ? koopaPosX - marioPosX : marioPosX - koopaPosX;
			auto isThatKoopaSide = collideCorrectX <= 14;

			auto collideCorrectY = (koopaPosY >= marioPosY) ? koopaPosY - marioPosY : marioPosY - koopaPosY;
			auto isThatKoopaFromUp = collideCorrectY <= 10;

			if ((marioType == SMALL_INVINCIBLE_MARIO ||
				 marioType == SUPER_INVINCIBLE_MARIO) &&
				 isThatKoopaSide && 
				 isThatKoopaFromUp)
			{
				koopa->SetState(KOOPATROOPA_DEAD);
				ADD_SCORE(100);

				return;
			}

			switch (state) 
			{
				case KOOPATROOPA_STUNNED:
					if (isThatKoopaSide && isThatKoopaFromUp)
					{
						PLAYSOUND("kick");
						remove(koopa);
						this->MakeKoopaTroopaStunnedMoving(this->GetType(),
							this->GetSprite()->GetPositionX(),
							this->GetSprite()->GetPositionY() - KOOPA_STUNNED_Y);
						return;
					}
					break;
				case KOOPATROOPA_WALKING_RIGHT:
				case KOOPATROOPA_WALKING_LEFT:
					if (marioState == MARIO_FALLING_LEFT || marioState == MARIO_FALLING_RIGHT)
					{
						if (isThatKoopaSide && isThatKoopaFromUp)
						{
							PLAYSOUND("stomp");
							remove(koopa);
							this->MakeKoopaTroopaStunned(this->GetType(),
								this->GetSprite()->GetPositionX(),
								this->GetSprite()->GetPositionY() + KOOPA_OFFSET);
							return;
						}
					}else
						if (marioDead(isThatKoopaSide && isThatKoopaFromUp))
							return;
					break;
				case KOOPATROOPA_STUNNED_MOVING:
					if (marioDead(isThatKoopaSide && isThatKoopaFromUp && youCanHitHim))
						return;
					break;
			}
		}
	});
}

const CollisionAction KoopaTroopa::MakeCollisionActionWithItem(KoopaTroopa* koopa)
{
	return [koopa](Sprite* s1, Sprite* s2)
	{
		if (!s1->GetVisibility() || !s2->GetVisibility())
			return;

		if (koopa->GetState() != KOOPATROOPA_WALKING_LEFT &&
			koopa->GetState() != KOOPATROOPA_WALKING_RIGHT)
			return;

		auto& bricks = Items::GetSigleton().GetAnimators();

		for (auto brick : bricks)
		{
			if (brick.first->GetTypeId() != R_BRICK_TYPE &&
				brick.first->GetTypeId() != Q_BRICK_TYPE)
				continue;

			auto brickPosX = brick.first->GetPositionX();
			auto brickPosY = brick.first->GetPositionY();

			auto koopaPosX = koopa->GetSprite()->GetPositionX();
			auto koopaPosY = koopa->GetSprite()->GetPositionY();

			auto collideCorrectY = (koopaPosY >= brickPosY) ? koopaPosY - brickPosY : brickPosY - koopaPosY;
			auto isThatGoombaFromUp = collideCorrectY <= 14 && collideCorrectY > 0;

			auto collideCorrectX = (koopaPosX >= brickPosX) ? koopaPosX - brickPosX : brickPosX - koopaPosX;
			auto isThatGoombaSide = collideCorrectX <= 14 && collideCorrectX > 0;

			if (isThatGoombaFromUp && isThatGoombaSide)
			{				
				koopa->SetState(KOOPATROOPA_DEAD);
				return;
			}
		}
	};
}

// s1 koopa , s2 goomba
const CollisionAction KoopaTroopa::MakeCollisionActionWithGoomba()
{
	return [](Sprite* s1, Sprite* s2)
	{
		if (!s1->GetVisibility() || !s2->GetVisibility())
			return;

		if (s1->GetTypeId().find(KOOPA_STUNNED) == std::string::npos)
			return;

		auto& goombas = goombaManager.GetAliveEnemies();

		for (auto* goomba : goombas)
		{
			if (goomba->GetState() != GOOMBA_WALKING)
				continue;

			auto goombaPosX = goomba->GetSprite()->GetPositionX();
			auto goombaPosY = goomba->GetSprite()->GetPositionY();

			auto koopaPosX = s1->GetPositionX();
			auto koopaPosY = s1->GetPositionY();

			auto collideCorrectY = (koopaPosY >= goombaPosY) ? koopaPosY - goombaPosY : goombaPosY - koopaPosY;
			auto isThatGoombaFromUp = collideCorrectY <= 14 && collideCorrectY > 0;

			auto collideCorrectX = (koopaPosX >= goombaPosX) ? koopaPosX - goombaPosX : goombaPosX - koopaPosX;
			auto isThatGoombaSide = collideCorrectX <= 14 && collideCorrectX > 0;

			if (isThatGoombaFromUp && isThatGoombaSide)
			{
				goomba->SetState(GOOMBA_DEAD);
				return;
			}
		}
	};
}

// s1 stunned koopa , s2 koopa
const CollisionAction KoopaTroopa::MakeCollisionActionWithKoopaTroopa()
{
	return [](Sprite* s1, Sprite* s2)
	{
		if (!s1->GetVisibility() || !s2->GetVisibility())
			return;

		if (s1->GetTypeId().find(KOOPA_STUNNED) == std::string::npos)
			return;

		auto& koopas = koopaTroopaManager.GetAliveEnemies();

		for (auto* koopa : koopas)
		{
			if ((koopa->GetState() != KOOPATROOPA_WALKING_LEFT && 
				 koopa->GetState() != KOOPATROOPA_WALKING_RIGHT) ||
				 s1 == koopa->GetSprite())
				continue;

			auto koopaPosX = koopa->GetSprite()->GetPositionX();
			auto koopaPosY = koopa->GetSprite()->GetPositionY();

			auto stunnedKoopaPosX = s1->GetPositionX();
			auto stunnedKoopaPosY = s1->GetPositionY();

			auto collideCorrectX = (koopaPosX >= stunnedKoopaPosX) ? koopaPosX - stunnedKoopaPosX : stunnedKoopaPosX - koopaPosX;
			auto isThatKoopaSide = collideCorrectX <= 14 && collideCorrectX >= 0;
			
			auto collideCorrectY = (koopaPosY >= stunnedKoopaPosY) ? koopaPosY - stunnedKoopaPosY : stunnedKoopaPosY - koopaPosY;
			auto isThatKoopaUp = collideCorrectY <= 14 && collideCorrectY >= 0;

			if (isThatKoopaSide && isThatKoopaUp)
			{
				SPRITE_MARK_AS_DEAD(koopa->GetSprite());
				ANIMATOR_MARK_AS_DEAD(koopa->GetAnimator());
				return;
			}
		}
	};
}

void KoopaTroopa::InitializeKoopaTroopa(KoopaTroopaType type, Dim x, Dim y)
{
	this->MakeKoopaTroopaWalkingRight(type , x , y);
}

void KoopaTroopa::MakeKoopaTroopaWalkingRight(KoopaTroopaType _type, Dim x, Dim y)
{
	GridLayer* grid = Terrain::GetActionLayer().GetGridLayer();

	std::string type(koopaTypes[_type] + KOOPA_WALKING_RIGHT);

	auto* isGreen = new bool(((type.find("green") != std::string::npos) ? true : false));

	this->SetState(KOOPATROOPA_WALKING_RIGHT);

	this->koopaFilm = new Sprite(GETFILM(type), x, y - KOOPA_OFFSET, type);
	this->koopaFilm->SetBoundingArea(new BoundingBox(x, y, x + TILE_WIDTH, y + TILE_HEIGHT + KOOPA_OFFSET));
	this->koopaFilm->SetMover(Sprite::MakeSpriteMover(grid, this->koopaFilm));
	
	Sprite::PrepareSpriteGravity(grid, this->koopaFilm);

	FrameRangeAnimation* koopaAnimation = new FrameRangeAnimation(type, 0, 2, 0, 4, 0, 70);

	this->koopaAnimator = new FrameRangeAnimator();

	this->koopaAnimator->SetOnAction([this,grid,isGreen](Animator* _anim, const Animation& _anima)
	{
		auto* frameAnimator = (FrameRangeAnimator*)_anim;
		auto& frameAnimation = (FrameRangeAnimation&)_anima;

		//change the koopas direction
		auto frameBox = this->koopaFilm->GetFrameBox();
		int x = frameAnimation.GetDx();
		int y = 0;

		grid->FilterGridMotion(frameBox, &x, &y);

		auto state = this->GetState();

		if (!(*isGreen)) 
		{
			auto koopaTileCol = (DIV_TILE_WIDTH(frameBox.x)) + 1;
			auto koopaTileRow = (DIV_TILE_HEIGHT(frameBox.y + frameBox.h + 1));

			auto isOnAir = (*(grid->GetGridTileBlock(koopaTileRow, koopaTileCol)) == GRID_EMPTY_TILE);

			if (isOnAir)
			{
				SPRITE_MARK_AS_DEAD(this->koopaFilm);
				ANIMATOR_MARK_AS_DEAD(this->koopaAnimator);
				this->MakeKoopaTroopaWalkingLeft(this->GetType(),
					this->koopaFilm->GetPositionX(),
					this->koopaFilm->GetPositionY() + KOOPA_OFFSET);
				return;
			}
		}

		if (x == 0 && (state != KOOPATROOPA_FALLING))
		{
			SPRITE_MARK_AS_DEAD(this->koopaFilm);
			ANIMATOR_MARK_AS_DEAD(this->koopaAnimator);
			this->MakeKoopaTroopaWalkingLeft(this->GetType(),
											 this->koopaFilm->GetPositionX(),
											 this->koopaFilm->GetPositionY() + KOOPA_OFFSET);
			return;
		}

		if ((frameAnimator->GetCurrFrame() != frameAnimation.GetStartFrame()) ||
			frameAnimator->GetCurrRep())
		{
			//change the frame and move the character
			this->koopaFilm->Move(frameAnimation.GetDx(), frameAnimation.GetDy());
		}

		this->koopaFilm->SetFrame(frameAnimator->GetCurrFrame());
		this->koopaFilm->SetBoundingAreaFromFrameBox();
	});

	this->koopaAnimator->SetOnFinish([](Animator* _anim)
	{
		auto* animator = (FrameRangeAnimator*)_anim;
		animator->SetLastTime(GETTIME);
	});

	this->koopaFilm->GetGravityHandler().SetOnStartFalling([this, koopaAnimation , isGreen]()
	{
		if ((*isGreen))
		{
			koopaAnimation->SetDx(0);
			koopaAnimation->SetDy(4);

			this->SetState(KOOPATROOPA_FALLING);
		}
	});


	this->koopaFilm->GetGravityHandler().SetOnStopFalling([this , koopaAnimation , isGreen]()
	{
		if ((*isGreen))
		{
			koopaAnimation->SetDx(4);
			koopaAnimation->SetDy(0);

			this->SetState(KOOPATROOPA_WALKING_RIGHT);
		}
	});

	for (auto* marioSprite : Mario::GetMario().GetMarioSprites()) 
		REGISTER(this->koopaFilm, marioSprite, this->MakeCollisionAction());
	for (auto brick : Items::GetSigleton().GetAnimators())
	{
		if (brick.first->GetTypeId() == Q_BRICK_TYPE || brick.first->GetTypeId() == R_BRICK_TYPE)
			REGISTER(this->koopaFilm, brick.first, this->MakeCollisionActionWithItem(this));
	}

	this->koopaAnimator->Start(koopaAnimation, GETTIME);
}

void KoopaTroopa::MakeKoopaTroopaWalkingLeft(KoopaTroopaType _type, Dim x, Dim y)
{
	GridLayer* grid = Terrain::GetActionLayer().GetGridLayer();

	std::string type(koopaTypes[_type] + KOOPA_WALKING_LEFT);

	auto* isGreen = new bool(((type.find("green") != std::string::npos) ? true : false));

	this->SetState(KOOPATROOPA_WALKING_LEFT);

	this->koopaFilm = new Sprite(GETFILM(type), x, y - KOOPA_OFFSET, type);
	this->koopaFilm->SetBoundingArea(new BoundingBox(x, y - KOOPA_OFFSET, x + TILE_WIDTH, y + TILE_HEIGHT + KOOPA_OFFSET));
	this->koopaFilm->SetMover(Sprite::MakeSpriteMover(grid, this->koopaFilm));

	Sprite::PrepareSpriteGravity(grid, this->koopaFilm);

	FrameRangeAnimation* koopaAnimation = new FrameRangeAnimation(type, 0, 2, 0, -4, 0, 70);

	this->koopaAnimator = new FrameRangeAnimator();

	this->koopaAnimator->SetOnAction([this, grid , isGreen](Animator* _anim, const Animation& _anima)
	{
		auto* frameAnimator = (FrameRangeAnimator*)_anim;
		auto& frameAnimation = (FrameRangeAnimation&)_anima;

		//change the koopas direction
		auto frameBox = this->koopaFilm->GetFrameBox();
		int x = frameAnimation.GetDx();
		int y = 0;

		grid->FilterGridMotion(frameBox, &x, &y);

		if (!(*isGreen))
		{
			auto koopaTileCol = (DIV_TILE_WIDTH(frameBox.x));
			auto koopaTileRow = (DIV_TILE_HEIGHT(frameBox.y + frameBox.h + 1));

			auto isOnAir = (*(grid->GetGridTileBlock(koopaTileRow, koopaTileCol)) == GRID_EMPTY_TILE);

			if (isOnAir)
			{
				SPRITE_MARK_AS_DEAD(this->koopaFilm);
				ANIMATOR_MARK_AS_DEAD(this->koopaAnimator);
				this->MakeKoopaTroopaWalkingRight(this->GetType(),
					this->koopaFilm->GetPositionX(),
					this->koopaFilm->GetPositionY() + KOOPA_OFFSET);
				return;
			}
		}

		if (x == 0 && (this->GetState() != KOOPATROOPA_FALLING))
		{
			SPRITE_MARK_AS_DEAD(this->koopaFilm);
			ANIMATOR_MARK_AS_DEAD(this->koopaAnimator);
			this->MakeKoopaTroopaWalkingRight(this->GetType(),
											  this->koopaFilm->GetPositionX(),
											  this->koopaFilm->GetPositionY() + KOOPA_OFFSET);
			return;
		}

		if ((frameAnimator->GetCurrFrame() != frameAnimation.GetStartFrame()) ||
			frameAnimator->GetCurrRep())
		{
			//change the frame and move the character
			this->koopaFilm->Move(frameAnimation.GetDx(), frameAnimation.GetDy());
		}

		this->koopaFilm->SetFrame(frameAnimator->GetCurrFrame());
		this->koopaFilm->SetBoundingAreaFromFrameBox();
	});

	this->koopaAnimator->SetOnFinish([](Animator* _anim)
	{
		auto* animator = (FrameRangeAnimator*)_anim;
		animator->SetLastTime(GETTIME);
	});

	this->koopaFilm->GetGravityHandler().SetOnStartFalling([this, koopaAnimation, isGreen]()
	{
		if ((*isGreen))
		{
			koopaAnimation->SetDx(0);
			koopaAnimation->SetDy(4);

			this->SetState(KOOPATROOPA_FALLING);
		}
	});

	this->koopaFilm->GetGravityHandler().SetOnStopFalling([this, koopaAnimation, isGreen]()
	{
		if ((*isGreen))
		{
			koopaAnimation->SetDx(-4);
			koopaAnimation->SetDy(0);

			this->SetState(KOOPATROOPA_WALKING_LEFT);
		}
	});

	for (auto* marioSprite : Mario::GetMario().GetMarioSprites())
		REGISTER(this->koopaFilm, marioSprite, this->MakeCollisionAction());
	for (auto brick : Items::GetSigleton().GetAnimators())
	{
		if (brick.first->GetTypeId() == Q_BRICK_TYPE || brick.first->GetTypeId() == R_BRICK_TYPE)
			REGISTER(this->koopaFilm, brick.first, this->MakeCollisionActionWithItem(this));
	}

	this->koopaAnimator->Start(koopaAnimation, GETTIME);
}

void KoopaTroopa::MakeKoopaTroopaStunned(KoopaTroopaType _type, Dim x, Dim y)
{
	GridLayer* grid = Terrain::GetActionLayer().GetGridLayer();

	std::string type(koopaTypes[_type] + KOOPA_STUNNED);

	this->SetState(KOOPATROOPA_STUNNED);

	auto x1 = x;
	auto y1 = y + KOOPA_STUNNED_Y;
	auto x2 = x + TILE_WIDTH - KOOPA_STUNNED_X;
	auto y2 = y + TILE_HEIGHT - KOOPA_STUNNED_Y;

	this->koopaFilm = new Sprite(GETFILM(type), x1, y1, type);
	this->koopaFilm->SetBoundingArea(new BoundingBox(x1 , y1 , x2 , y2));
	this->koopaFilm->SetMover(Sprite::MakeSpriteMover(grid, this->koopaFilm));

	Sprite::PrepareSpriteGravity(grid, this->koopaFilm);

	FrameRangeAnimation* koopaAnimation = new FrameRangeAnimation(type, 0, 2, 1, 0, 0, 100);

	this->koopaAnimator = new FrameRangeAnimator();

	this->koopaAnimator->SetOnAction([this, grid](Animator* _anim, const Animation& _anima)
	{
		auto* frameAnimator = (FrameRangeAnimator*)_anim;
		auto& frameAnimation = (FrameRangeAnimation&)_anima;

		this->koopaFilm->SetFrame(frameAnimator->GetCurrFrame());
		this->koopaFilm->SetBoundingAreaFromFrameBox();
	});

	for (auto* marioSprite : Mario::GetMario().GetMarioSprites())
		REGISTER(this->koopaFilm, marioSprite, this->MakeCollisionAction());
	
	//=============================TickAnimation=========================

	TickAnimation* koopaStunned = new TickAnimation(type, STUNNED_DELAY, 1, true);
	TickAnimator* koopaStunnedAnima = new TickAnimator();

	koopaStunnedAnima->SetOnFinish([this](Animator* _anim)
	{
		PLAYSOUND("stomp");

		SPRITE_MARK_AS_DEAD(this->GetSprite());
		ANIMATOR_MARK_AS_DEAD(this->GetAnimator());

		this->MakeKoopaTroopaWalkingRight(this->GetType(),
										  this->GetSprite()->GetPositionX(),
										  this->GetSprite()->GetPositionY() - KOOPA_STUNNED_Y);
	});

	TickAnimation* koopaCanHit = new TickAnimation(type, 1000, 1, true);
	TickAnimator* koopaCanHitAnima = new TickAnimator();
	youCanHitHim = false;

	koopaCanHitAnima->SetOnFinish([this](Animator* _anim)
	{
		youCanHitHim = true;
	});

	this->koopaAnimator->Start(koopaAnimation, GETTIME);
	koopaStunnedAnima->Start(*koopaStunned, GETTIME);
	koopaCanHitAnima->Start(*koopaCanHit, GETTIME);
}

void KoopaTroopa::MakeKoopaTroopaStunnedMoving(KoopaTroopaType _type, Dim x, Dim y)
{
	GridLayer* grid = Terrain::GetActionLayer().GetGridLayer();

	std::string type(koopaTypes[_type] + KOOPA_STUNNED);

	this->SetState(KOOPATROOPA_STUNNED_MOVING);

	auto x1 = x;
	auto y1 = y + KOOPA_STUNNED_Y;
	auto x2 = x + TILE_WIDTH - KOOPA_STUNNED_X;
	auto y2 = y + TILE_HEIGHT - KOOPA_STUNNED_Y;

	this->koopaFilm = new Sprite(GETFILM(type), x1, y1, type);
	this->koopaFilm->SetBoundingArea(new BoundingBox(x1, y1, x2, y2));
	this->koopaFilm->SetMover(Sprite::MakeSpriteMover(grid, this->koopaFilm));

	Sprite::PrepareSpriteGravity(grid, this->koopaFilm);

	FrameRangeAnimation* koopaAnimation = new FrameRangeAnimation(type, 0, 2, 0, 4, 0, 40);

	this->koopaFilm->GetGravityHandler().SetOnStartFalling([this,koopaAnimation]()
	{
		koopaAnimation->SetDx(0);
		koopaAnimation->SetDy(4);

		this->SetState(KOOPATROOPA_FALLING);
	});

	this->koopaFilm->GetGravityHandler().SetOnStopFalling([this,koopaAnimation]()
	{
		koopaAnimation->SetDx(4);
		koopaAnimation->SetDy(0);

		this->SetState(KOOPATROOPA_STUNNED_MOVING);
	});

	this->koopaAnimator = new FrameRangeAnimator();

	this->koopaAnimator->SetOnAction([this, grid](Animator* _anim, const Animation& _anima)
	{
		auto* frameAnimator = (FrameRangeAnimator*)_anim;
		auto& frameAnimation = (FrameRangeAnimation&)_anima;

		//change the koopas direction
		int x = frameAnimation.GetDx();
		int y = 0;

		grid->FilterGridMotion(this->koopaFilm->GetFrameBox(), &x, &y);

		if (x == 0)
			frameAnimation.SetDx(-frameAnimation.GetDx());
		
		if ((frameAnimator->GetCurrFrame() != frameAnimation.GetStartFrame()) ||
			frameAnimator->GetCurrRep())
		{
			//change the frame and move the character
			this->koopaFilm->Move(frameAnimation.GetDx(), frameAnimation.GetDy());
		}

		this->koopaFilm->SetFrame(frameAnimator->GetCurrFrame());
		this->koopaFilm->SetBoundingAreaFromFrameBox();
	});

	for (auto* marioSprite : Mario::GetMario().GetMarioSprites())
		REGISTER(this->koopaFilm, marioSprite, this->MakeCollisionAction());
	for (auto* goomba : goombaManager.GetAliveEnemies())
		REGISTER(this->koopaFilm, goomba->GetSprite(), this->MakeCollisionActionWithGoomba());
	for (auto* koopa : koopaTroopaManager.GetAliveEnemies())
		if (koopa != this)
			REGISTER(this->koopaFilm, koopa->GetSprite(), this->MakeCollisionActionWithKoopaTroopa());

	//=============================TickAnimation=========================

	TickAnimation* koopaStunned = new TickAnimation(type, STUNNED_DELAY, 1, true);
	TickAnimator* koopaStunnedAnima = new TickAnimator();

	koopaStunnedAnima->SetOnFinish([this](Animator* _anim)
	{
		PLAYSOUND("stomp");

		SPRITE_MARK_AS_DEAD(this->GetSprite());
		ANIMATOR_MARK_AS_DEAD(this->GetAnimator());

		this->MakeKoopaTroopaWalkingRight(this->GetType(),
			this->GetSprite()->GetPositionX(),
			this->GetSprite()->GetPositionY() - KOOPA_STUNNED_Y);
	});

	this->koopaAnimator->Start(koopaAnimation, GETTIME);
	koopaStunnedAnima->Start(*koopaStunned, GETTIME);
}

KoopaTroopa::KoopaTroopa(KoopaTroopaType type, Dim x, Dim y)
{
	this->koopaType = type;

	this->InitializeKoopaTroopa(type, x, y);

	koopaTroopaManager.Register(this);
}

KoopaTroopaState KoopaTroopa::GetState()
{
	return this->koopaState;
}

void KoopaTroopa::SetState(KoopaTroopaState _state)
{
	this->prevState = this->koopaState;
	this->koopaState = _state;
}

KoopaTroopaType KoopaTroopa::GetType()
{
	return this->koopaType;
}

void KoopaTroopa::SetType(KoopaTroopaType _type)
{
	this->koopaType = _type;
}

Sprite* KoopaTroopa::GetSprite()
{
	return this->koopaFilm;
}

FrameRangeAnimator* KoopaTroopa::GetAnimator()
{
	return this->koopaAnimator;
}

bool KoopaTroopa::IsDead() const
{
	return this->koopaState == KOOPATROOPA_DEAD;
}
