#include "Mario.h"

Mario Mario::mario;

void Mario::PrepareMarioSpriteGravityHandler(GridLayer* layer, Sprite* sprite)
{
	sprite->GetGravityHandler().SetIsGravityAddicted(true);

	sprite->GetGravityHandler().SetOnSolidGround([layer,this](const Rect& r)
	{
		auto marioTileCol  = DIV_TILE_WIDTH(r.x);
		auto marioTileRow  = DIV_TILE_HEIGHT(r.y + r.h + 1);

		auto isOnSolid = *(layer->GetGridTileBlock(marioTileRow, marioTileCol))     == GRID_SOLID_TILE &&
						 *(layer->GetGridTileBlock(marioTileRow, marioTileCol + 1)) == GRID_EMPTY_TILE &&
						 *(layer->GetGridTileBlock(marioTileRow, marioTileCol + 2)) == GRID_SOLID_TILE;

		if ((this->GetMarioState() == MARIO_RUNNING_RIGHT || 
			 this->GetMarioState() == MARIO_RUNNING_LEFT) && 
			 isOnSolid) 
		{
			return true;
		}

		return layer->IsOnSolidGround(r);
	});
}

//--------------------------------------------------ANIMATIONS-------------------------------------------

void Mario::InitializeMarioRightFalling()
{
	std::string type("mario.falling_right");

	this->marioFallingRightFilm = new Sprite(GETFILM(type), 0 , 192 , type);
	this->marioFallingRightFilm->SetBoundingArea(new BoundingBox(0 , 192 , 16 , 208));
	this->marioFallingRightFilm->SetMover(Sprite::MakeSpriteMover(this->gridLayer, this->marioFallingRightFilm));
	
	this->PrepareMarioSpriteGravityHandler(this->gridLayer, this->marioFallingRightFilm);
	
	this->marioFallingRightFilm->GetGravityHandler().SetOnStopFalling([this](void)
	{
		this->onAir = false;
		this->SetFilmsPositionY(this->marioFallingRightFilm->GetPositionY());

		SUSPEND(this->marioFallingRight);
		//TODO check here if mario is dead to act differently
		RUN(this->marioStandingRight);

		SETSTATE(MARIO_STANDING_RIGHT);

		this->marioFallingRight->SetLastTime(GETTIME);
	});

	MovingAnimation* marioFallingAnimation = new MovingAnimation(type, 0, 0, 4, 50);

	this->marioFallingRight = new MovingAnimator();

	this->marioFallingRight->SetOnAction([this](Animator*  animator, const Animation& animation) 
	{
		auto& movingAnimation = (MovingAnimation&)animation;
		
		this->onAir = true;
		this->SetFilmsPositionY(this->marioFallingRightFilm->GetPositionY());

		this->marioFallingRightFilm->Move(movingAnimation.GetDx(), movingAnimation.GetDy());
		this->marioFallingRightFilm->SetBoundingAreaFromFrameBox();
	});

	this->marioFallingRight->Start(marioFallingAnimation, GETTIME);

	SUSPEND(this->marioFallingRight);
}

void Mario::InitializeMarioLeftFalling() 
{
	std::string type("mario.falling_left");

	this->marioFallingLeftFilm = new Sprite(GETFILM(type), 0, 192, type);
	this->marioFallingLeftFilm->SetBoundingArea(new BoundingBox(0, 192, 16, 208));
	this->marioFallingLeftFilm->SetMover(Sprite::MakeSpriteMover(this->gridLayer, this->marioFallingLeftFilm));

	this->PrepareMarioSpriteGravityHandler(this->gridLayer, this->marioFallingLeftFilm);

	this->marioFallingLeftFilm->GetGravityHandler().SetOnStopFalling([this](void)
	{
		this->onAir = false;
		this->SetFilmsPositionY(this->marioFallingLeftFilm->GetPositionY());

		SUSPEND(this->marioFallingLeft);
		//TODO check here if mario is dead to act differently
		RUN(this->marioStandingLeft);

		SETSTATE(MARIO_STANDING_LEFT);

		this->marioFallingLeft->SetLastTime(GETTIME);
	});

	MovingAnimation* marioFallingAnimation = new MovingAnimation(type, 0, 0, 4, 50);

	this->marioFallingLeft = new MovingAnimator();

	this->marioFallingLeft->SetOnAction([this](Animator*  animator, const Animation& animation)
	{
		auto& movingAnimation = (MovingAnimation&)animation;

		this->onAir = true;
		this->SetFilmsPositionY(this->marioFallingLeftFilm->GetPositionY());

		this->marioFallingLeftFilm->Move(movingAnimation.GetDx(), movingAnimation.GetDy());
		this->marioFallingLeftFilm->SetBoundingAreaFromFrameBox();
	});

	this->marioFallingLeft->Start(marioFallingAnimation, GETTIME);

	SUSPEND(this->marioFallingLeft);
}

void Mario::InitializeMarioStandingRight()
{
	std::string type("mario.standing_right");

	this->marioStandingRightFilm = new Sprite(GETFILM(type), 0, 192, type);
	this->marioStandingRightFilm->SetBoundingArea(new BoundingBox(0, 192, 16, 208));
	this->marioStandingRightFilm->SetMover(Sprite::MakeSpriteMover(this->gridLayer , this->marioStandingRightFilm));
	
	this->onAir = false;
	
	this->PrepareMarioSpriteGravityHandler(this->gridLayer, this->marioStandingRightFilm);

	this->marioStandingRightFilm->GetGravityHandler().SetOnStartFalling([this]()
	{
		this->onAir = true;
		this->SetFilmsPositionY(this->marioStandingRightFilm->GetPositionY());

		SUSPEND(this->marioStandingRight);
		RUN(this->marioFallingRight);

		SETSTATE(MARIO_FALLING_RIGHT);
	});

	MovingAnimation* marioStandingAnimation = new MovingAnimation(type , 1 , 0 , 0 , 100);

	this->marioStandingRight = new MovingAnimator();

	this->marioStandingRight->SetOnAction([this](Animator* _animator, const Animation& _animation) 
	{
		auto* animator = (MovingAnimator*)_animator;

		if (this->IsDying())
		{
			this->MarioDie();
			SUSPEND(animator);
			return;
		}

		this->marioStandingRightFilm->SetBoundingAreaFromFrameBox();
	});

	this->marioStandingRight->Start(marioStandingAnimation , GETTIME);
}

void Mario::InitializeMarioStandingLeft()
{
	std::string type("mario.standing_left");

	this->marioStandingLeftFilm = new Sprite(GETFILM(type), 0, 192, type);
	this->marioStandingLeftFilm->SetBoundingArea(new BoundingBox(0, 192, 16, 208));
	this->marioStandingLeftFilm->SetMover(Sprite::MakeSpriteMover(this->gridLayer, this->marioStandingLeftFilm));

	this->onAir = false;

	this->PrepareMarioSpriteGravityHandler(this->gridLayer, this->marioStandingLeftFilm);

	this->marioStandingLeftFilm->GetGravityHandler().SetOnStartFalling([this]()
	{
		this->onAir = true;
		this->SetFilmsPositionY(this->marioStandingLeftFilm->GetPositionY());

		SUSPEND(this->marioStandingLeft);
		RUN(this->marioFallingLeft);

		SETSTATE(MARIO_FALLING_LEFT);
	});

	MovingAnimation* marioStandingAnimation = new MovingAnimation(type, 1, 0, 0, 100);

	this->marioStandingLeft = new MovingAnimator();

	this->marioStandingLeft->SetOnAction([this](Animator* _animator, const Animation& _animation)
	{
		auto* animator = (MovingAnimator*)_animator;

		if (this->IsDying())
		{
			this->MarioDie();
			SUSPEND(animator);
			return;
		}

		this->marioStandingLeftFilm->SetBoundingAreaFromFrameBox();
	});

	this->marioStandingLeft->Start(marioStandingAnimation, GETTIME);
}

void Mario::InitializeMarioWalkingRight()
{
	std::string type("mario.walking_right");

	this->marioWalkingRightFilm = new Sprite(GETFILM(type), 0, 192, type);
	this->marioWalkingRightFilm->SetBoundingArea(new BoundingBox(0, 192, 16, 208));
	this->marioWalkingRightFilm->SetMover(Sprite::MakeSpriteMover(this->gridLayer, this->marioWalkingRightFilm));
	
	this->PrepareMarioSpriteGravityHandler(this->gridLayer, this->marioWalkingRightFilm);

	this->marioWalkingRightFilm->GetGravityHandler().SetOnStartFalling([this]()
	{
		this->onAir = true;
		this->SetFilmsPositionY(this->marioWalkingRightFilm->GetPositionY());

		SUSPEND(this->marioWalkingRight);
		RUN(this->marioFallingRight);

		SETSTATE(MARIO_FALLING_RIGHT);
	});

	FrameRangeAnimation* marioWalkingAnimation = new FrameRangeAnimation(type, 0, 3, 1, 2, 0, 10);

	this->marioWalkingRight= new FrameRangeAnimator();

	this->marioWalkingRight->SetOnAction([this , marioWalkingAnimation](Animator* animator , const Animation& animation)
	{
		auto* frameAnimator	 = (FrameRangeAnimator*)animator;
		auto& frameAnimation = (FrameRangeAnimation&)(const_cast<Animation&>(animation));

		if (this->IsDying())
		{
			this->MarioDie();
			SUSPEND(animator);
			return;
		}

		if ((frameAnimator->GetCurrFrame() != frameAnimation.GetStartFrame()) || 
			 frameAnimator->GetCurrRep()) 
		{
			//change the frame and move the character
			this->marioWalkingRightFilm->Move(frameAnimation.GetDx(), frameAnimation.GetDy());
		}

		this->marioWalkingRightFilm->SetFrame(frameAnimator->GetCurrFrame());
		this->marioWalkingRightFilm->SetBoundingAreaFromFrameBox();
	});

	this->marioWalkingRight->SetOnFinish([this](Animator* animator)
	{
		this->SetFilmsPositionX(this->marioWalkingRightFilm->GetPositionX());

		//Set the mario state to stading
		if (!onAir) {
			SETSTATE(MARIO_STANDING_RIGHT);
		
			//SUSPEND(this->marioWalkingRight);
			//SUSPEND(this->marioJumpingRight);
			RUN(this->marioStandingRight);
		}

		auto* frameAnimator = (FrameRangeAnimator*)animator;
		frameAnimator->SetLastTime(GETTIME);
	});

	this->marioWalkingRight->Start(marioWalkingAnimation, GETTIME);
	//Suspend walking
	SUSPEND(this->marioWalkingRight);
}

void Mario::InitializeMarioWalkingLeft()
{
	std::string type("mario.walking_left");

	this->marioWalkingLeftFilm = new Sprite(GETFILM(type), 0, 192, type);
	this->marioWalkingLeftFilm->SetBoundingArea(new BoundingBox(0, 192, 16, 208));
	this->marioWalkingLeftFilm->SetMover(Sprite::MakeSpriteMover(this->gridLayer, this->marioWalkingLeftFilm));
	
	this->PrepareMarioSpriteGravityHandler(this->gridLayer, this->marioWalkingLeftFilm);

	this->marioWalkingLeftFilm->GetGravityHandler().SetOnStartFalling([this]()
	{
		this->onAir = true;

		this->SetFilmsPositionY(this->marioWalkingLeftFilm->GetPositionY());

		SUSPEND(this->marioWalkingLeft);
		RUN(this->marioFallingLeft);

		SETSTATE(MARIO_FALLING_LEFT);
	});

	FrameRangeAnimation* marioWalkingAnimation = new FrameRangeAnimation(type, 0, 3, 1, -2, 0, 10);

	this->marioWalkingLeft = new FrameRangeAnimator();

	this->marioWalkingLeft->SetOnAction([this](Animator* animator, const Animation& animation)
	{
		auto* frameAnimator = (FrameRangeAnimator*)animator;
		auto& frameAnimation = (FrameRangeAnimation&)(const_cast<Animation&>(animation));

		if (this->IsDying())
		{
			this->MarioDie();
			SUSPEND(animator);
			return;
		}

		if ((frameAnimator->GetCurrFrame() != frameAnimation.GetStartFrame()) ||
			frameAnimator->GetCurrRep())
		{
			//change the frame and move the character
			this->marioWalkingLeftFilm->Move(frameAnimation.GetDx(), frameAnimation.GetDy());
		}

		this->marioWalkingLeftFilm->SetFrame(frameAnimator->GetCurrFrame());
		this->marioWalkingLeftFilm->SetBoundingAreaFromFrameBox();
	});

	this->marioWalkingLeft->SetOnFinish([this](Animator* animator)
	{
		this->SetFilmsPositionX(this->marioWalkingLeftFilm->GetPositionX());

		if (!onAir) {
			//Set the mario state to stading
			SETSTATE(MARIO_STANDING_LEFT);

			//SUSPEND(this->marioWalkingLeft);
			//SUSPEND(this->marioJumpingLeft);
			RUN(this->marioStandingLeft);
		}
		auto* frameAnimator = (FrameRangeAnimator*)animator;
		frameAnimator->SetLastTime(GETTIME);
	});

	this->marioWalkingLeft->Start(marioWalkingAnimation, GETTIME);
	//Suspend walking
	SUSPEND(this->marioWalkingLeft);
}

void Mario::InitializeMarioJumpingRight()
{
	std::string type("mario.jumping_right");

	this->marioJumpingRightFilm = new Sprite(GETFILM(type), 0, 192, type);
	this->marioJumpingRightFilm->SetBoundingArea(new BoundingBox(0, 192, 0, 208));
	this->marioJumpingRightFilm->SetMover(Sprite::MakeSpriteMover(this->gridLayer, this->marioJumpingRightFilm));

	this->PrepareMarioSpriteGravityHandler(this->gridLayer, this->marioJumpingRightFilm);

	std::vector<PathEntry> pathToFollow;
	for (auto i = 0; i < 40; i++)
	{
		pathToFollow.push_back(PathEntry(0, -2, 0, 10));

	}

	MovingPathAnimation* marioJumpingAnimation = new MovingPathAnimation(type, pathToFollow);

	this->marioJumpingRight = new MovingPathAnimator();
	
	this->marioJumpingRight->SetOnAction([this](Animator* animator, const Animation& animation)
	{
		auto* pathAnimator = (MovingPathAnimator*)animator;
		auto& pathAnimation = (MovingPathAnimation&)(const_cast<Animation&>(animation));

		this->onAir = true;

		this->marioJumpingRightFilm->GetGravityHandler().SetIsGravityAddicted(false);
		this->SetFilmsPositionY(this->marioJumpingRightFilm->GetPositionY());

		//change the frame and move the character
		auto pathPos = pathAnimator->GetCurrPathPos();

		this->marioJumpingRightFilm->Move(pathAnimation.GetPath()[pathPos].dx, pathAnimation.GetPath()[pathPos].dy);
		this->marioJumpingRightFilm->SetBoundingAreaFromFrameBox();
	});

	this->marioJumpingRight->SetOnFinish([this](Animator* animator)
	{
		this->onAir = true;

		this->SetFilmsPositionY(this->marioJumpingRightFilm->GetPositionY());
		this->marioJumpingRightFilm->GetGravityHandler().SetIsGravityAddicted(true);

		SETSTATE(MARIO_FALLING_RIGHT);

		//SUSPEND(this->marioJumpingRight);
		//SUSPEND(this->marioWalkingRight);
		RUN(this->marioFallingRight);

		auto* movingPathAnimator = (MovingPathAnimator*)animator;
		movingPathAnimator->SetLastTime(GETTIME);
	});

	this->marioJumpingRight->Start(marioJumpingAnimation, GETTIME);
	//Suspend jumping
	SUSPEND(this->marioJumpingRight);
}

void Mario::InitializeMarioJumpingLeft()
{
	std::string type("mario.jumping_left");

	this->marioJumpingLeftFilm = new Sprite(GETFILM(type), 0, 192, type);
	this->marioJumpingLeftFilm->SetBoundingArea(new BoundingBox(0, 192, 16, 208));
	this->marioJumpingLeftFilm->SetMover(Sprite::MakeSpriteMover(this->gridLayer, this->marioJumpingLeftFilm));

	this->PrepareMarioSpriteGravityHandler(this->gridLayer, this->marioJumpingLeftFilm);

	std::vector<PathEntry> pathToFollow;
	for (auto i = 0; i < 40; i++)
	{
		pathToFollow.push_back(PathEntry(0, -2, 0, 10));
	}

	MovingPathAnimation* marioJumpingAnimation = new MovingPathAnimation(type, pathToFollow);

	this->marioJumpingLeft = new MovingPathAnimator();

	this->marioJumpingLeft->SetOnAction([this](Animator* animator, const Animation& animation)
	{
		auto* pathAnimator = (MovingPathAnimator*)animator;
		auto& pathAnimation = (MovingPathAnimation&)(const_cast<Animation&>(animation));

		this->onAir = true;
		this->marioJumpingLeftFilm->GetGravityHandler().SetIsGravityAddicted(false);
		this->SetFilmsPositionY(this->marioJumpingLeftFilm->GetPositionY());

		//change the frame and move the character
		auto pathPos = pathAnimator->GetCurrPathPos();

		this->marioJumpingLeftFilm->Move(pathAnimation.GetPath()[pathPos].dx, pathAnimation.GetPath()[pathPos].dy);
		this->marioJumpingLeftFilm->SetBoundingAreaFromFrameBox();
	});

	this->marioJumpingLeft->SetOnFinish([this](Animator* animator)
	{
		this->onAir = true;

		this->SetFilmsPositionY(this->marioJumpingLeftFilm->GetPositionY());
		this->marioJumpingLeftFilm->GetGravityHandler().SetIsGravityAddicted(true);

		SETSTATE(MARIO_FALLING_LEFT);

		//SUSPEND(this->marioJumpingLeft);
		RUN(this->marioFallingLeft);

		auto* movingPathAnimator = (MovingPathAnimator*)animator;
		movingPathAnimator->SetLastTime(GETTIME);
	});

	this->marioJumpingLeft->Start(marioJumpingAnimation, GETTIME);
	//Suspend jumping
	SUSPEND(this->marioJumpingLeft);
}

void Mario::MarioDie()
{
	std::string type("mario.dying");

	auto currentSprite = this->GetMarioSprite();
	auto x = currentSprite->GetPositionX();
	auto y = currentSprite->GetPositionY();
	auto w = currentSprite->GetFrameBox().w;
	auto h = currentSprite->GetFrameBox().h;

	this->SetMarioState(MARIO_DYING);

	this->marioDyingFilm = new Sprite(GETFILM(type), x, y, type);
	this->marioDyingFilm->SetBoundingArea(new BoundingBox(x, y, x + w , y + h));
	this->marioDyingFilm->SetHasDirectMotion(true);

	std::vector<PathEntry> pathToFollow;
	for (auto i = 0; i < 15; i++)
	{
		pathToFollow.push_back(PathEntry(0, -4, 0, 50));
	}
	for (auto i = 0; i < 40; i++) {
		pathToFollow.push_back(PathEntry(0, 4, 0, 50));
	}

	MovingPathAnimation* marioDyingAnimation = new MovingPathAnimation(type, pathToFollow);

	this->marioDying = new MovingPathAnimator();

	this->marioDying->SetOnAction([this](Animator* animator, const Animation& animation)
	{
		auto* pathAnimator = (MovingPathAnimator*)animator;
		auto& pathAnimation = (MovingPathAnimation&)(const_cast<Animation&>(animation));

		this->SetMarioState(MARIO_DYING);

		//change the frame and move the character
		auto pathPos = pathAnimator->GetCurrPathPos();

		this->marioDyingFilm->Move(pathAnimation.GetPath()[pathPos].dx, pathAnimation.GetPath()[pathPos].dy);
		this->marioDyingFilm->SetBoundingAreaFromFrameBox();
	});

	this->marioDying->SetOnFinish([this](Animator* animator)
	{	
		if (ScoreLayer::GetLives() == 0)
		{
			mario.SetMarioState(MARIO_DEAD);
		}

		if (this->GetMarioState() != MARIO_DEAD)
		{
			this->SetFilmsPosition(this->checkpoints[this->currentCheckpoint] , 192);
			this->SetFilmsBoundingArea();
			this->SetMarioState(MARIO_STANDING_RIGHT);
		}
	});

	this->marioDying->Start(marioDyingAnimation, GETTIME);
}

void Mario::MarioEnterPipe()
{
	if (this->marioState == MARIO_DYING || 
		this->marioState == MARIO_DEAD  ||
		this->marioState == MARIO_ENTER_PIPE)
		return;

	std::string type("mario.enter_pipe");

	this->marioEnterPipe = new Sprite(GETFILM(type), this->GetMarioSprite()->GetPositionX(), this->GetMarioSprite()->GetPositionY(), type);
	marioEnterPipe->SetHasDirectMotion(true);

	this->SetMarioState(MARIO_ENTER_PIPE);

	MovingAnimation* marioEnterPipeAnimation = new MovingAnimation(type, 8, 0, 4, 80);
	
	this->marioEnterPipeAnimator = new MovingAnimator();

	marioEnterPipeAnimator->SetOnAction([this](Animator* _animator , const Animation& _animation) 
	{
		auto& animation = (MovingAnimation&)_animation;

		marioEnterPipe->Move(animation.GetDx(), animation.GetDy());
		marioEnterPipe->SetBoundingAreaFromFrameBox();
	});

	marioEnterPipeAnimator->SetOnFinish([this](Animator* _animator) 
	{
		auto& actionLayer = Terrain::GetActionLayer();
		auto& scenes	  = actionLayer.GetScene();
		
		auto newScene = 1 - actionLayer.GetCurrentScene();
		auto scene	  = scenes[newScene];

		this->SetFilmsPosition(scene.marioPosX, scene.marioPosY);
		this->SetFilmsBoundingArea();

		actionLayer.SetViewWindow(Rect{scene.tileX * TILE_WIDTH,
									   scene.tileY * TILE_HEIGHT,
									   scene.viewWindowWidth,
									   scene.viewWindowHeight });
		
		this->isMarioOnPipe = false;
		
		actionLayer.SetDisplayChanged(true);

		if (actionLayer.GetCurrentScene() == 0)
		{
			RUN(this->marioFallingRight);
			this->SetMarioState(MARIO_FALLING_RIGHT);
		}

		actionLayer.SetCurrentScene(newScene);
	});

	PLAYSOUND("pipe");

	marioEnterPipeAnimator->Start(marioEnterPipeAnimation, GETTIME);
}

void Mario::MarioExitPipe()
{
	if (this->marioState == MARIO_DYING ||
		this->marioState == MARIO_DEAD ||
		this->marioState == MARIO_ENTER_PIPE)
		return;

	std::string type("mario.enter_pipe");

	this->marioEnterPipe = new Sprite(GETFILM(type), this->GetMarioSprite()->GetPositionX(), this->GetMarioSprite()->GetPositionY(), type);
	marioEnterPipe->SetHasDirectMotion(true);

	this->SetMarioState(MARIO_ENTER_PIPE);

	MovingAnimation* marioEnterPipeAnimation = new MovingAnimation(type, 8, 4, 0, 80);

	this->marioEnterPipeAnimator = new MovingAnimator();

	marioEnterPipeAnimator->SetOnAction([this](Animator* _animator, const Animation& _animation)
	{
		auto& animation = (MovingAnimation&)_animation;

		marioEnterPipe->Move(animation.GetDx(), animation.GetDy());
		marioEnterPipe->SetBoundingAreaFromFrameBox();
	});

	marioEnterPipeAnimator->SetOnFinish([this](Animator* _animator)
	{
		auto& actionLayer = Terrain::GetActionLayer();
		auto& scenes = actionLayer.GetScene();

		auto newScene = 1 - actionLayer.GetCurrentScene();
		auto scene = scenes[newScene];

		this->SetFilmsPosition(scene.marioPosX, scene.marioPosY);
		this->SetFilmsBoundingArea();

		actionLayer.SetViewWindow(Rect{scene.tileX * TILE_WIDTH,
									   scene.tileY * TILE_HEIGHT,
									   scene.viewWindowWidth,
									   scene.viewWindowHeight });

		this->isMarioOnPipe = false;

		actionLayer.SetDisplayChanged(true);

		this->SetMarioState(MARIO_STANDING_RIGHT);

		actionLayer.SetCurrentScene(newScene);
	});

	PLAYSOUND("pipe");

	marioEnterPipeAnimator->Start(marioEnterPipeAnimation, GETTIME);
}

void Mario::InitializeMarioSprites()
{
	this->marioFilms.push_back(this->marioWalkingLeftFilm);
	this->marioFilms.push_back(this->marioWalkingRightFilm);
	this->marioFilms.push_back(this->marioFallingLeftFilm);
	this->marioFilms.push_back(this->marioFallingRightFilm);
	this->marioFilms.push_back(this->marioJumpingLeftFilm);
	this->marioFilms.push_back(this->marioJumpingRightFilm);
	this->marioFilms.push_back(this->marioStandingLeftFilm);
	this->marioFilms.push_back(this->marioStandingRightFilm);
}

void Mario::CheckForCheckPoint()
{
	if (currentCheckpoint < this->checkpoints.size() - 1  && this->GetMarioSprite()->GetPositionX() >= this->checkpoints[this->currentCheckpoint + 1])
		this->currentCheckpoint++;
}

void Mario::MakeInvisibleMarioTimer()
{
#define INVISIBLE_MARIO_DELAY 30000
	auto type = this->GetMarioType();

	if (type != SUPER_INVINCIBLE_MARIO || type != SMALL_INVINCIBLE_MARIO)
		return;

	TickAnimation* marioInvisible = new TickAnimation("invisible_mario" , INVISIBLE_MARIO_DELAY , 1 , true);

	TickAnimator* invisibleAnimator = new TickAnimator();

	invisibleAnimator->SetOnFinish([this](Animator* anim) 
	{
		ANIMATOR_MARK_AS_DEAD(anim);

		this->SetMarioType(this->oldType);
	});

	invisibleAnimator->Start(*marioInvisible, GETTIME);

#undef INVISIBLE_MARIO_DELAY
}

//--------------------------------------------------COLLISION---------------------------------------------

const CollisionAction Mario::MakePowerUpAction(MovingAnimator* _anim)
{
	return ([this, _anim](Sprite* s1, Sprite* s2)
	{
		if (!s1->GetVisibility())
			return;

		if (this->GetMarioSprite() != s2)
			return;

		auto& running = Items::GetSigleton().GetPowerUpAnimators();

		auto marioP = s2->GetFrameBox();

		for (ItemAnimators::iterator iter = running.begin(); iter != running.end(); ++iter)
		{
			auto type = iter->first->GetTypeId();

			auto powerUpX = iter->first->GetPositionX();
			auto powerUpY = iter->first->GetPositionY();

			auto collideLeft = ((powerUpX >= marioP.x) && ((marioP.x + marioP.w) >= powerUpX));
			auto collideRight = ((powerUpX <= marioP.x) && ((powerUpX + TILE_WIDTH) >= marioP.x));
			auto collideCorrectX = (powerUpX >= marioP.x) ? powerUpX - marioP.x : marioP.x - powerUpX;
			auto isThatPowerUpSide = collideCorrectX <= 5;

			auto collideUp = ((powerUpY >= marioP.y) && ((marioP.y + marioP.h) >= powerUpY));
			auto collideDown = ((powerUpY <= marioP.y) && ((powerUpY + TILE_HEIGHT) >= marioP.y));
			auto collideCorrectY = (powerUpY >= marioP.y) ? powerUpY - marioP.y : marioP.y - powerUpY;
			auto isThatPowerUp = collideCorrectY <= 5;

			if (((collideLeft || collideRight) && isThatPowerUpSide) ||
				((collideUp || collideDown) && isThatPowerUp))
			{
				//Delete the coin's sprite ,animator 
				ANIMATOR_MARK_AS_DEAD(iter->second);
				SPRITE_MARK_AS_DEAD(s1);

				iter = running.erase(iter);

				ADD_SCORE(1000);

				if (type == MUSHROOM_TYPE)
				{
					PLAYSOUND("power_up");
					switch (this->GetMarioType())
					{
					case SMALL_MARIO:
						this->SetMarioType(SUPER_MARIO);
						break;
					case SMALL_INVINCIBLE_MARIO:
						this->SetMarioType(SUPER_INVINCIBLE_MARIO);
						break;
					}
				}
				else if (type == ONE_UP_TYPE)
				{
					PLAYSOUND("1_up");
					ADD_LIVE(1);
				}
				else if (type == STARMAN_TYPE)
				{
					PLAYSOUND("power_up");
					switch (this->GetMarioType())
					{
						case SUPER_MARIO:
							this->SetMarioType(SUPER_INVINCIBLE_MARIO);

							//Start the timer
							this->MakeInvisibleMarioTimer();
							break;
						case SMALL_MARIO:
							this->SetMarioType(SMALL_INVINCIBLE_MARIO);

							//Start the timer
							this->MakeInvisibleMarioTimer();
							break;
					}
				}
				return;
			}
		}
	});
}

void Mario::InitializeMarioRBrickCollision()
{
	auto& rbricks = SpriteManager::GetSigleton().GetTypeList(R_BRICK_TYPE);

	auto rbrick_action = [this](Sprite* s1, Sprite* s2)
	{
		if (!s1->GetVisibility())
			return;

		if (this->GetMarioSprite() != s2)
			return;

		Items& items = Items::GetSigleton();

		auto& running = items.GetAnimators();

		auto marioP = s2->GetFrameBox();

		for (ItemAnimators::iterator animator = running.begin(); animator != running.end(); animator++) 
		{
			auto type = animator->first->GetTypeId();

			if (animator->first->GetStateId() == DEAD_STATE)
				return;

			if (type != R_BRICK_TYPE)
				continue;

			auto qbrickX = animator->first->GetPositionX();
			auto qbrickY = animator->first->GetPositionY();

			auto collideDown = ((qbrickY <= marioP.y) && ((qbrickY + TILE_HEIGHT) >= marioP.y));

			auto collideCorrect = (qbrickX >= marioP.x) ? qbrickX - marioP.x : marioP.x - qbrickX;
			auto isThatBrick = collideCorrect <= 6;

			if (isThatBrick && collideDown)
			{
				TileLayer& actionLayer = Terrain::GetActionLayer();
				
				switch (this->GetMarioType())
				{
					case SUPER_MARIO:
					case SUPER_INVINCIBLE_MARIO:

						PLAYSOUND("break_block");

						items.GenerateABrickBreakAnimation(qbrickX, qbrickY);

						actionLayer.SetTile(qbrickY / TILE_HEIGHT, qbrickX / TILE_WIDTH, BACKGROUND_TILE);
						actionLayer.GetGridLayer()->SetGridTileBlock(qbrickY / TILE_HEIGHT, qbrickX / TILE_WIDTH, GRID_EMPTY_TILE);

						/*SPRITE_MARK_AS_DEAD(s1);
						ANIMATOR_MARK_AS_DEAD(animator->second);

						animator = running.erase(animator);*/

						s1->SetStateId(DEAD_STATE);

						break;
					case SMALL_MARIO:
					case SMALL_INVINCIBLE_MARIO:
						items.GenerateABounceBrickAnimation(dynamic_cast<MovingPathAnimator*>(animator->second));
						break;
				}

				if (ISSTATE(MARIO_JUMPING_RIGHT)) 
				{
					RUN(this->marioFallingRight);
					SUSPEND(this->marioJumpingRight);
					SETSTATE(MARIO_FALLING_RIGHT);
				}
				else
				{
					RUN(this->marioFallingLeft);
					SUSPEND(this->marioJumpingLeft);
					SETSTATE(MARIO_FALLING_LEFT);
				}

				return;
			}
		}
	};

	for (auto* rbrick : rbricks)
	{
		REGISTER(rbrick, this->marioJumpingRightFilm, rbrick_action);
		REGISTER(rbrick, this->marioJumpingLeftFilm, rbrick_action);
	}
}

void Mario::InitializeMarioQBrickCollision()
{
	auto& qbricks = SpriteManager::GetSigleton().GetTypeList(Q_BRICK_TYPE);

	auto qbrick_action = [this](Sprite* s1, Sprite* s2)
	{
		if (!s1->GetVisibility())
			return;

		if (this->GetMarioSprite() != s2)
			return;

		Items& items = Items::GetSigleton();

		auto& running = items.GetAnimators();

		auto marioP = s2->GetFrameBox();

		for (ItemAnimators::iterator animator = running.begin(); animator != running.end(); animator++)
		{
			auto type = animator->first->GetTypeId();

			if (type != Q_BRICK_TYPE)
				continue;

			auto qbrickX = animator->first->GetPositionX();
			auto qbrickY = animator->first->GetPositionY();

			auto collideDown = ((qbrickY <= marioP.y) && ((qbrickY + TILE_HEIGHT) >= marioP.y));

			auto collideCorrect = (qbrickX >= marioP.x) ? qbrickX - marioP.x : marioP.x - qbrickX;
			auto isThatBrick = collideCorrect <= 8;

			if (isThatBrick && collideDown)
			{
				items.GenerateABounceBrickAnimation(dynamic_cast<MovingPathAnimator*>(animator->second));

				auto result = items.IsPowerUpActive(qbrickX, qbrickY);

				if (result == 0)
				{
					std::string powerUp = items.ActivatePowerUp(qbrickX, qbrickY);

					if (powerUp == COIN_TYPE)
					{
						PLAYSOUND("coin");

						items.GenerateFloatingCoin(qbrickX, qbrickY);

						ADD_COIN(1);
						ADD_SCORE(200);

						if (ScoreLayer::GetCoins() == 100)
						{
							ADD_LIVE(1);
							ScoreLayer::SetCoins(0);
						}
					}
					else
					{
						PLAYSOUND("power_up_appears");

						PowerUpPair powerup = items.GeneratePowerUp(qbrickX, qbrickY, powerUp);

						REGISTER(powerup.first, this->marioStandingRightFilm, this->MakePowerUpAction(powerup.second));
						REGISTER(powerup.first, this->marioStandingLeftFilm, this->MakePowerUpAction(powerup.second));
						REGISTER(powerup.first, this->marioWalkingRightFilm, this->MakePowerUpAction(powerup.second));
						REGISTER(powerup.first, this->marioWalkingLeftFilm, this->MakePowerUpAction(powerup.second));
						REGISTER(powerup.first, this->marioJumpingRightFilm, this->MakePowerUpAction(powerup.second));
						REGISTER(powerup.first, this->marioJumpingLeftFilm, this->MakePowerUpAction(powerup.second));
						REGISTER(powerup.first, this->marioFallingRightFilm, this->MakePowerUpAction(powerup.second));
						REGISTER(powerup.first, this->marioFallingLeftFilm, this->MakePowerUpAction(powerup.second));
					}
				}

				s1->SetStateId(DEAD_STATE);

				if (ISSTATE(MARIO_JUMPING_RIGHT)) 
				{
					RUN(this->marioFallingRight);
					SUSPEND(this->marioJumpingRight);
					SETSTATE(MARIO_FALLING_RIGHT);
				}
				else 
				{
					RUN(this->marioFallingLeft);
					SUSPEND(this->marioJumpingLeft);
					SETSTATE(MARIO_FALLING_LEFT);
				}

				items.GenerateAEmptyBrickAnimation(qbrickX , qbrickY);

				return;
			}
		}
	};

	for (auto* qbrick : qbricks)
	{
		REGISTER(qbrick, this->marioJumpingRightFilm, qbrick_action);
		REGISTER(qbrick, this->marioJumpingLeftFilm, qbrick_action);
		REGISTER(qbrick, this->marioFallingRightFilm, qbrick_action);
		REGISTER(qbrick, this->marioFallingLeftFilm, qbrick_action);
	}
}

void Mario::InitializeMarioCoinCollision()
{
	auto& coins = SpriteManager::GetSigleton().GetTypeList(COIN_TYPE);

	auto coin_action = [this](Sprite* s1, Sprite* s2)
	{
		if (!s1->GetVisibility())
			return;

		if (this->GetMarioSprite() != s2)
			return;

		auto& running = Items::GetSigleton().GetAnimators();

		auto marioP = s2->GetFrameBox();

		for (ItemAnimators::iterator animator = running.begin(); animator != running.end(); animator++)
		{
			if (animator->first->GetStateId() == DEAD_STATE)
				continue;

			auto type = animator->first->GetTypeId();

			if (type != COIN_TYPE)
				continue;

			auto coinX = animator->first->GetPositionX();
			auto coinY = animator->first->GetPositionY();

			auto collideCorrectSide = (coinX >= marioP.x) ? coinX - marioP.x : marioP.x - coinX;
			auto isThatCoinSide= collideCorrectSide <= 15;

			auto collideCorrectUp = (coinY >= marioP.y) ? coinY - marioP.y : marioP.y - coinY;
			auto isThatCoinUp = collideCorrectUp <= 5;

			if ( isThatCoinSide  &&  isThatCoinUp)
			{
				PLAYSOUND("coin");

				s1->SetStateId(DEAD_STATE);

				ADD_COIN(1);
				ADD_SCORE(200);

				if (ScoreLayer::GetCoins() == 100)
				{
					ADD_LIVE(1);
					ScoreLayer::SetCoins(0);
				}
				return;
			}
		}
	};

	for (auto* coin : coins)
	{
		REGISTER(coin, this->marioStandingRightFilm, coin_action);
		REGISTER(coin, this->marioStandingLeftFilm, coin_action);
		REGISTER(coin, this->marioWalkingRightFilm, coin_action);
		REGISTER(coin, this->marioWalkingLeftFilm, coin_action);
		REGISTER(coin, this->marioJumpingRightFilm, coin_action);
		REGISTER(coin, this->marioJumpingLeftFilm, coin_action);
		REGISTER(coin, this->marioFallingRightFilm, coin_action);
		REGISTER(coin, this->marioFallingLeftFilm, coin_action);
	}
}

void Mario::InitializeMarioPipeCollision() 
{
	auto pipes = Items::GetSigleton().GetPipes();
	
	auto piranha_action = [this](Sprite* s1, Sprite* s2)
	{
		auto state = this->GetMarioState();

		if (!s1->GetVisibility())
			return;

		if (this->GetMarioSprite() != s2)
			return;

		if (state == MARIO_DYING ||
			state == MARIO_DEAD)
			return;

		auto marioFrame = s2->GetFrameBox();

		auto pipePosX  = s1->GetPositionX();
		auto pipePosY  = s1->GetPositionY();

		auto hasPipePiranhaQuestionMark = (s1->GetTypeId().find("pipe_up_left") != std::string::npos);
		if (hasPipePiranhaQuestionMark)
		{
			auto& piranhas = piranhasManager.GetAliveEnemies();

			auto correctX = marioFrame.x - pipePosX;
			auto correctY = pipePosY - (marioFrame.y + marioFrame.h);

			auto isMarioOnPipeWithPiranha = (correctX <= 25 && correctX >= 0) && correctY == 0;

			((isMarioOnPipeWithPiranha) ? this->isMarioOnPipe = true : this->isMarioOnPipe = false);

			for (auto piranha : piranhas)
			{
				if (piranha->GetState() == PIRANHA_DEAD)
					continue;

				auto piranhaPosX = piranha->GetSprite()->GetPositionX();
				auto piranhaPosY = piranha->GetSprite()->GetPositionY();

				auto correctX = (pipePosX - piranhaPosX) >= 0 ? pipePosX - piranhaPosX : piranhaPosX - pipePosX;
				auto isThatPiranha = correctX <= 20;

				if (isThatPiranha && isMarioOnPipeWithPiranha /*&& piranha->GetState() == PIRANHA_DOWN*/)
					SUSPEND(piranha->GetAnimator());
				else
					RUN(piranha->GetAnimator());
			}
		}
	};

	for (auto pipe : pipes)
		if (pipe.second) 
		{
			REGISTER(pipe.first, this->marioStandingRightFilm, piranha_action);
			REGISTER(pipe.first, this->marioStandingLeftFilm, piranha_action);
		}
}

void Mario::MarioInitializeCollision()
{
	this->InitializeMarioCoinCollision();

	this->InitializeMarioRBrickCollision();

	this->InitializeMarioQBrickCollision();

	this->InitializeMarioPipeCollision();
}

//--------------------------------------------------INITIALIZE--------------------------------------------

void Mario::InitializeMario()
{
	this->gridLayer = Terrain::GetActionLayer().GetGridLayer();

	InitializeMarioRightFalling();
	InitializeMarioLeftFalling();

	InitializeMarioStandingRight();
	InitializeMarioStandingLeft();

	InitializeMarioWalkingRight();
	InitializeMarioWalkingLeft();

	InitializeMarioJumpingRight();
	InitializeMarioJumpingLeft();

	InitializeMarioSprites();

	MarioInitializeCollision();
}

//--------------------------------------------------SETERS/GETERS-----------------------------------------

void Mario::SetFilmsPosition(Dim _x, Dim _y)
{
	this->marioStandingRightFilm->SetPosition(_x, _y);
	this->marioStandingLeftFilm->SetPosition(_x, _y);

	this->marioFallingRightFilm->SetPosition(_x, _y);
	this->marioFallingLeftFilm->SetPosition(_x, _y);

	this->marioWalkingRightFilm->SetPosition(_x, _y);
	this->marioWalkingLeftFilm->SetPosition(_x, _y);

	this->marioJumpingRightFilm->SetPosition(_x, _y);
	this->marioJumpingLeftFilm->SetPosition(_x, _y);
}

void Mario::SetFilmsBoundingArea()
{
	this->marioStandingRightFilm->SetBoundingAreaFromFrameBox();
	this->marioStandingLeftFilm->SetBoundingAreaFromFrameBox();
	
	this->marioFallingRightFilm->SetBoundingAreaFromFrameBox();
	this->marioFallingLeftFilm->SetBoundingAreaFromFrameBox();
	
	this->marioWalkingRightFilm->SetBoundingAreaFromFrameBox();
	this->marioWalkingLeftFilm->SetBoundingAreaFromFrameBox();
	
	this->marioJumpingRightFilm->SetBoundingAreaFromFrameBox();
	this->marioJumpingLeftFilm->SetBoundingAreaFromFrameBox();
}

void Mario::SetFilmsPositionX(Dim _x)
{
	this->marioStandingRightFilm->SetPositionX(_x);
	this->marioStandingLeftFilm->SetPositionX(_x);

	this->marioFallingRightFilm->SetPositionX(_x);
	this->marioFallingLeftFilm->SetPositionX(_x);

	this->marioWalkingRightFilm->SetPositionX(_x);
	this->marioWalkingLeftFilm->SetPositionX(_x);

	this->marioJumpingRightFilm->SetPositionX(_x);
	this->marioJumpingLeftFilm->SetPositionX(_x);
}

void Mario::SetFilmsPositionY(Dim _y)
{
	this->marioStandingRightFilm->SetPositionY(_y);
	this->marioStandingLeftFilm->SetPositionY(_y);

	this->marioFallingRightFilm->SetPositionY(_y);
	this->marioFallingLeftFilm->SetPositionY(_y);

	this->marioWalkingRightFilm->SetPositionY(_y);
	this->marioWalkingLeftFilm->SetPositionY(_y);

	this->marioJumpingRightFilm->SetPositionY(_y);
	this->marioJumpingLeftFilm->SetPositionY(_y);
}

void Mario::AddFilmsPositionX(Dim _dx)
{
	this->marioStandingRightFilm->SetPositionX(this->marioStandingRightFilm->GetPositionX() + _dx);
	this->marioStandingLeftFilm->SetPositionX(this->marioStandingLeftFilm->GetPositionX()   + _dx);

	this->marioFallingRightFilm->SetPositionX(this->marioFallingRightFilm->GetPositionX()   + _dx);
	this->marioFallingLeftFilm->SetPositionX( this->marioFallingLeftFilm->GetPositionX()    + _dx);

	this->marioWalkingRightFilm->SetPositionX(this->marioWalkingRightFilm->GetPositionX()   + _dx);
	this->marioWalkingLeftFilm->SetPositionX( this->marioWalkingLeftFilm->GetPositionX()    + _dx);

	this->marioJumpingRightFilm->SetPositionX(this->marioJumpingRightFilm->GetPositionX()   + _dx);
	this->marioJumpingLeftFilm->SetPositionX( this->marioJumpingLeftFilm->GetPositionX()    + _dx);
}

void Mario::SetMarioState(MarioState _state)
{
	this->oldState		= this->marioState;
	this->marioState	= _state;
}

MarioState& Mario::GetMarioState()
{
	return marioState;
}

void Mario::SetMarioType(MarioType _type)
{
	this->oldType   = this->marioType;
	this->marioType = _type;
}

MarioType& Mario::GetMarioType()
{
	return this->marioType;
}

bool Mario::IsDead() const
{
	return this->marioState == MARIO_DEAD;
}

bool Mario::IsDying() const
{
	return this->marioState == MARIO_DYING;
}

bool Mario::IsMarioOnPipe() const
{
	return this->isMarioOnPipe;
}

bool Mario::CheckMarioOnSecondPipe()
{
	return  Terrain::GetActionLayer().GetCurrentScene() == 1 &&
			this->GetMarioSprite()->GetPositionX() >= 13 * TILE_WIDTH &&
			this->GetMarioSprite()->GetPositionY() == 192;
}

bool Mario::IsSuperMario() const
{
	return (this->marioType == SUPER_INVINCIBLE_MARIO ||
			this->marioType == SUPER_MARIO);
}

void Mario::MakeMarioDie()
{
	if (this->marioState == MARIO_DYING || 
		this->marioState == MARIO_DEAD  || 
		this->marioState == MARIO_ENTER_PIPE)
		return;

	if (this->GetMarioSprite()->GetPositionY() > 13 * TILE_HEIGHT)
	{
		PLAYSOUND("mario_die");
	}
}

Sprite* Mario::GetMarioSprite()
{
	switch (this->marioState) 
	{
		case MARIO_FALLING_LEFT:
			return this->marioFallingLeftFilm;
		case MARIO_FALLING_RIGHT:
			return this->marioFallingRightFilm;
		case MARIO_WALKING_LEFT:
		case MARIO_RUNNING_LEFT:
			return this->marioWalkingLeftFilm;
		case MARIO_WALKING_RIGHT:
		case MARIO_RUNNING_RIGHT:
			return this->marioWalkingRightFilm;
		case MARIO_JUMPING_LEFT:
			return this->marioJumpingLeftFilm;
		case MARIO_JUMPING_RIGHT:
			return this->marioJumpingRightFilm;
		case MARIO_STANDING_LEFT:
			return this->marioStandingLeftFilm;
		case MARIO_STANDING_RIGHT:
			return this->marioStandingRightFilm;
		case MARIO_DYING:
			return this->marioDyingFilm;
		case MARIO_ENTER_PIPE:
			return this->marioEnterPipe;
	}

	return nullptr;
}

MarioSprites& Mario::GetMarioSprites()
{
	this->marioFilms.clear();

	this->InitializeMarioSprites();

	return this->marioFilms;
}

MarioSprites& Mario::GetMarioWalkingSprites()
{
	this->marioFilms.clear();

	this->marioFilms.push_back(this->marioWalkingLeftFilm);
	this->marioFilms.push_back(this->marioWalkingRightFilm);

	return this->marioFilms;
}

MarioSprites& Mario::GetMarioFallingSprites()
{
	this->marioFilms.clear();

	this->marioFilms.push_back(this->marioFallingLeftFilm);
	this->marioFilms.push_back(this->marioFallingRightFilm);

	return this->marioFilms;
}

MarioSprites& Mario::GetMarioJumpingSprites()
{
	this->marioFilms.clear();

	this->marioFilms.push_back(this->marioJumpingLeftFilm);
	this->marioFilms.push_back(this->marioJumpingRightFilm);

	return this->marioFilms;
}

MarioSprites& Mario::GetMarioStandingSprites()
{
	this->marioFilms.clear();

	this->marioFilms.push_back(this->marioStandingLeftFilm);
	this->marioFilms.push_back(this->marioStandingRightFilm);

	return this->marioFilms;
}

Mario& Mario::GetMario()
{
	return mario;
}

//--------------------------------------------------MOVEMENT-----------------------------------------------

void Mario::MarioWalkRight()
{
	if (!ISSTATE(MARIO_DYING) && !ISSTATE(MARIO_DEAD) && !ISSTATE(MARIO_JUMPING_LEFT) && !ISSTATE(MARIO_FALLING_LEFT))
	{
		RUN(this->marioWalkingRight);

		if (!this->onAir)
		{
			
			this->marioWalkingRight->GetAnim()->SetDelay(30);
			this->marioWalkingRight->GetAnim()->SetDx(4);
			
			SETSTATE(MARIO_WALKING_RIGHT);
		}
		else
		{
			this->marioWalkingRight->GetAnim()->SetDelay(10);
			this->marioWalkingRight->GetAnim()->SetDx(2);
			SETSTATE(MARIO_JUMPING_RIGHT);
		}
	}
}

void Mario::MarioWalkLeft()
{
	if (!ISSTATE(MARIO_DYING) && !ISSTATE(MARIO_DEAD) && !ISSTATE(MARIO_JUMPING_RIGHT) && !ISSTATE(MARIO_FALLING_RIGHT))
	{
		RUN(this->marioWalkingLeft);

		if (!this->onAir)
		{
			this->marioWalkingLeft->GetAnim()->SetDelay(30);
			this->marioWalkingLeft->GetAnim()->SetDx(-4);
			SETSTATE(MARIO_WALKING_LEFT);
		}
		else
		{
			this->marioWalkingLeft->GetAnim()->SetDelay(10);
			this->marioWalkingLeft->GetAnim()->SetDx(-2);
			SETSTATE(MARIO_JUMPING_LEFT);
		}
	}
}

void Mario::MarioRunRight()
{
	if (!ISSTATE(MARIO_DYING) && !ISSTATE(MARIO_DEAD))
	{
		RUN(this->marioWalkingRight);

		if (!this->onAir)
		{
			this->marioWalkingRight->GetAnim()->SetDelay(15);
			this->marioWalkingRight->GetAnim()->SetDx(3);
			SETSTATE(MARIO_RUNNING_RIGHT);
		}
		else {
			this->marioWalkingRight->GetAnim()->SetDelay(9);
			this->marioWalkingRight->GetAnim()->SetDx(3);
			SETSTATE(MARIO_JUMPING_RIGHT);
		}
	}
}

void Mario::MarioRunLeft()
{
	if (!ISSTATE(MARIO_DYING) && !ISSTATE(MARIO_DEAD)) 
	{
		RUN(this->marioWalkingLeft);

		if (!this->onAir)
		{
			this->marioWalkingLeft->GetAnim()->SetDelay(15);
			this->marioWalkingLeft->GetAnim()->SetDx(-3);
			SETSTATE(MARIO_RUNNING_LEFT);
		}
		else {
			this->marioWalkingLeft->GetAnim()->SetDelay(9);
			this->marioWalkingLeft->GetAnim()->SetDx(-3);
			SETSTATE(MARIO_JUMPING_LEFT);
		}
	}
}

void Mario::MarioStandingAndJumping()
{
	if (!ISSTATE(MARIO_DYING) && !ISSTATE(MARIO_DEAD))
	{
		if (!this->onAir)
		{
			if (this->IsSuperMario())
				PLAYSOUND("super_mario_jump");
			else
				PLAYSOUND("mario_jump");

			if (ISSTATE(MARIO_STANDING_RIGHT) || ISSTATE(MARIO_WALKING_RIGHT))
			{
				RUN(this->marioJumpingRight);
				SETSTATE(MARIO_JUMPING_RIGHT);
			}
			else if (ISSTATE(MARIO_STANDING_LEFT) || ISSTATE(MARIO_WALKING_LEFT))
			{
				RUN(this->marioJumpingLeft);
				SETSTATE(MARIO_JUMPING_LEFT);
			}
		}
	}
}

//--------------------------------------------------DRAW---------------------------------------------------

void Mario::Draw(Bitmap at)
{
	if (this->GetMarioSprite() == nullptr)
		return;

	if (!this->GetMarioSprite()->GetVisibility())
		return;

	this->CheckForCheckPoint();

	switch (this->marioState)
	{
		case MARIO_WALKING_RIGHT:
		case MARIO_RUNNING_RIGHT:
			//this->marioWalkingRightFilm->Display(at, Terrain::GetActionLayer().GetViewWindow(), Clipper().SetView([]() { return Terrain::GetActionLayer().GetViewWindow(); }));
			this->marioWalkingRightFilm->Display(at);
			break;
		case MARIO_WALKING_LEFT:
		case MARIO_RUNNING_LEFT:
			//this->marioWalkingLeftFilm->Display(at, Terrain::GetActionLayer().GetViewWindow(), Clipper().SetView([]() { return Terrain::GetActionLayer().GetViewWindow(); }));
			this->marioWalkingLeftFilm->Display(at);
			break;
		case MARIO_JUMPING_RIGHT:
			//this->marioJumpingRightFilm->Display(at, Terrain::GetActionLayer().GetViewWindow(), Clipper().SetView([]() { return Terrain::GetActionLayer().GetViewWindow(); }));
			this->marioJumpingRightFilm->Display(at);
			break;
		case MARIO_JUMPING_LEFT:
			//this->marioJumpingLeftFilm->Display(at, Terrain::GetActionLayer().GetViewWindow(), Clipper().SetView([]() { return Terrain::GetActionLayer().GetViewWindow(); }));
			this->marioJumpingLeftFilm->Display(at);
			break;
		case MARIO_STANDING_RIGHT:
			//this->marioStandingRightFilm->Display(at, Terrain::GetActionLayer().GetViewWindow(), Clipper().SetView([]() { return Terrain::GetActionLayer().GetViewWindow(); }));
			this->marioStandingRightFilm->Display(at);
			break;
		case MARIO_STANDING_LEFT:
			//this->marioStandingLeftFilm->Display(at, Terrain::GetActionLayer().GetViewWindow(), Clipper().SetView([]() { return Terrain::GetActionLayer().GetViewWindow(); }));
			this->marioStandingLeftFilm->Display(at);
			break;
		case MARIO_FALLING_RIGHT:
			//this->marioFallingRightFilm->Display(at, Terrain::GetActionLayer().GetViewWindow(), Clipper().SetView([]() { return Terrain::GetActionLayer().GetViewWindow(); }));
			this->marioFallingRightFilm->Display(at);
			break;
		case MARIO_FALLING_LEFT:
			//this->marioFallingLeftFilm->Display(at, Terrain::GetActionLayer().GetViewWindow(), Clipper().SetView([]() { return Terrain::GetActionLayer().GetViewWindow(); }));
			this->marioFallingLeftFilm->Display(at);
			break;
		case MARIO_DYING:
			//this->marioDyingFilm->Display(at, Terrain::GetActionLayer().GetViewWindow(), Clipper().SetView([]() { return Terrain::GetActionLayer().GetViewWindow(); }));
			this->marioDyingFilm->Display(at);
			break;
		case MARIO_ENTER_PIPE:
			//this->marioEnterPipe->Display(at, Terrain::GetActionLayer().GetViewWindow(), Clipper().SetView([]() { return Terrain::GetActionLayer().GetViewWindow(); }));
			this->marioEnterPipe->Display(at);
			break;
		default:
			assert(0);
	}
}

//--------------------------------------------------CONSTRUCTOR--------------------------------------------

Mario::Mario() 
{
	this->oldState	 = MARIO_STANDING_RIGHT;
	this->marioState = MARIO_STANDING_RIGHT;
	this->marioType  = SMALL_MARIO;
	this->oldType	 = SMALL_MARIO;
}
