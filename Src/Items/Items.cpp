#include "Items.h"

Items Items::items;

//--------------------------------------------------POWER-UPS--------------------------------------------

int Items::IsPowerUpActive(Dim x, Dim y)
{
	for (auto powerup : this->powerUps)
		if (powerup.first.first->GetPositionX() == x &&
			powerup.first.first->GetPositionY() == y)
		{
			return ((powerup.second) ? 1 : 0);
		}
	return -1;
}

std::string Items::ActivatePowerUp(Dim x, Dim y)
{
	for (auto& powerup : this->powerUps)
		if (powerup.first.first->GetPositionX() == x &&
			powerup.first.first->GetPositionY() == y)
		{
			powerup.second = true;
			return std::string(powerup.first.second);
		}
	return std::string(NONE_TYPE);
}

void Items::ReadPowerUps()
{
	std::ifstream fileToRead;
	std::string line;

	Dim row = 0;
	Dim col = 0;

	Dim backgroundTile  = 47;
	Dim coinTile		= 231;
	Dim mashRoomTile    = 234;
	Dim oneUpTile		= 235;
	Dim starManTile		= 236;

	fileToRead.open(POWER_UPS_PATH, std::ios::in);
	if (!fileToRead.is_open())
	{
		std::cerr << "Failed to open the tile map file for reading\n";
		return;
	}

	while (std::getline(fileToRead, line))
	{
		std::istringstream	lineStream(line);
		std::string			cell;

		col = 0;
		while (std::getline(lineStream, cell, ','))
		{
			Dim index = static_cast<Dim>(std::stoi(cell));
			if (index != 47)
			{
				auto x = col * TILE_WIDTH;
				auto y = row * TILE_HEIGHT;
				std::string type("");

				if (index == mashRoomTile)
					type = std::string(MUSHROOM_TYPE);
				else if (index == oneUpTile)
					type = std::string(ONE_UP_TYPE);
				else if (index == starManTile)
					type = std::string(STARMAN_TYPE);
				else if (index == coinTile)
					type = std::string(COIN_TYPE);

				for(auto item : this->animators)
					if (item.first->GetPositionX() == x && item.first->GetPositionY() == y && item.first->GetTypeId() == Q_BRICK_TYPE) 
					{
						this->powerUps[std::make_pair(item.first, type)] = false;
					}
			}
			col++;
		}
		row++;
	}

	fileToRead.close();
}

PowerUpPair Items::GeneratePowerUp(Dim x, Dim y, std::string _type)
{
	GridLayer* grid = Terrain::GetActionLayer().GetGridLayer();
	auto _dx		= (_type == STARMAN_TYPE) ? 4 : 3;

	Sprite* powerUp = new Sprite(GETFILM(_type), x, y - TILE_HEIGHT , _type);
	powerUp->SetBoundingArea(new BoundingBox(x, y - TILE_HEIGHT, x + TILE_WIDTH, y));
	powerUp->SetMover(Sprite::MakeSpriteMover(grid, powerUp));
	Sprite::PrepareSpriteGravity(grid, powerUp);

	MovingAnimation* movingAnimation = new MovingAnimation(_type, 0, _dx, 0, 40);

	powerUp->GetGravityHandler().SetOnStartFalling([movingAnimation , &_type]() 
	{
		auto _dy = (_type == STARMAN_TYPE) ? 4 : 3;

		movingAnimation->SetDx(0);
		movingAnimation->SetDy(_dy);
	});
	
	powerUp->GetGravityHandler().SetOnStopFalling([movingAnimation, &_type]()
	{
		auto _dx = (_type == STARMAN_TYPE) ? 4 : 3;

		movingAnimation->SetDx(_dx);
		movingAnimation->SetDy(0);
	});

	MovingAnimator* movingAnimator = new MovingAnimator();

	this->powerUpsAnimators[powerUp] = movingAnimator;

	movingAnimator->SetOnAction([powerUp , grid](Animator* _animator, const Animation& _animation)
	{
		auto& movingAnimation = (MovingAnimation&)_animation;
		
		//change the powerUp's direction
		int x = movingAnimation.GetDx();
		int y = 0;

		grid->FilterGridMotion(powerUp->GetFrameBox(), &x, &y);

		if (x == 0)
			movingAnimation.SetDx(-movingAnimation.GetDx());

		powerUp->Move(movingAnimation.GetDx(), movingAnimation.GetDy());
		powerUp->SetBoundingAreaFromFrameBox();
	});

	movingAnimator->Start(movingAnimation, GETSYSTEMTIME);

	return std::make_pair(powerUp, movingAnimator);
}

void Items::GenerateABounceBrickAnimation(MovingPathAnimator* _animator)
{
	assert(_animator);

	Path path;
	for (auto pos = 0; pos < 8; ++pos)
	{
		if (pos < 4)	path.push_back(PathEntry(0, -2, 0, 40));
		else			path.push_back(PathEntry(0, 2, 0, 40));
	}

	_animator->GetAnim()->SetPath(path);
	_animator->GetAnim()->SetReps(1);
	_animator->SetCurrPathPos(0);
	_animator->SetCurrRep(0);

	_animator->SetOnFinish([](Animator* _animator) 
	{
		Path path;
		path.push_back(PathEntry(0, 0, 0, 500));

		auto* pathAnimator = (MovingPathAnimator*)_animator;
		
		pathAnimator->GetAnim()->SetPath(path);
		pathAnimator->SetCurrPathPos(0);
		pathAnimator->SetCurrRep(0);
		pathAnimator->GetAnim()->SetReps(0);

		pathAnimator->SetOnFinish([](Animator* _animator) {});
	});

	_animator->Start(_animator->GetAnim(), GETTIME);
}

void Items::GenerateABrickBreakAnimation(Dim x, Dim y)
{
#define BRICK_OFFSET 8

	std::string type(R_BRICK_BREAK);

	Sprite* left_brick = new Sprite(GETFILM(type), x - BRICK_OFFSET, y, type);
	left_brick->SetHasDirectMotion(true);
	Sprite* right_brick = new Sprite(GETFILM(type), x - BRICK_OFFSET, y + TILE_HEIGHT, type);
	right_brick->SetHasDirectMotion(true);
	Sprite* down_left_brick = new Sprite(GETFILM(type), x + TILE_WIDTH, y, type);
	down_left_brick->SetHasDirectMotion(true);
	Sprite* down_right_brick = new Sprite(GETFILM(type), x + TILE_WIDTH, y + TILE_HEIGHT, type);
	down_right_brick->SetHasDirectMotion(true);

	const auto brick_action = ([](Sprite* s)
	{
		return ([s](Animator* _animator, const Animation& _animation)
		{
			auto* pathAnimator = (MovingPathAnimator*)_animator;
			const auto& pathAnimation = (MovingPathAnimation&)_animation;

			auto pathPos = pathAnimator->GetCurrPathPos();
			auto path = pathAnimation.GetPath();

			s->Move(path[pathPos].dx, path[pathPos].dy);
		});
	});

	const auto brick_finish = ([](Sprite* s)
	{
		return ([s](Animator* _animator)
		{
			ANIMATOR_MARK_AS_DEAD(_animator);
			SPRITE_MARK_AS_DEAD(s);
		});
	});

	const auto generate_path([](int _dx, int _dy)
	{
		Path path;

		for (auto pos = 0; pos < 30; pos++)
		{
			if (pos < 5) path.push_back(PathEntry(_dx, -_dy, 0, 40));
			else        path.push_back(PathEntry(_dx, _dy, 0, 40));
		}

		return path;
	});

	MovingPathAnimation* leftBreakAnimation = new MovingPathAnimation(type, generate_path(-2, 6));
	MovingPathAnimation* dleftBreakAnimation = new MovingPathAnimation(type, generate_path(2, 6));
	MovingPathAnimation* rightBreakAnimation = new MovingPathAnimation(type, generate_path(-1, 6));
	MovingPathAnimation* drightBreakAnimation = new MovingPathAnimation(type, generate_path(1, 6));

	MovingPathAnimator* leftBreakAnimator = new MovingPathAnimator();
	MovingPathAnimator* rightBreakAnimator = new MovingPathAnimator();
	MovingPathAnimator* dleftBreakAnimator = new MovingPathAnimator();
	MovingPathAnimator* drightBreakAnimator = new MovingPathAnimator();

	leftBreakAnimator->SetOnAction(brick_action(left_brick));
	rightBreakAnimator->SetOnAction(brick_action(right_brick));
	dleftBreakAnimator->SetOnAction(brick_action(down_left_brick));
	drightBreakAnimator->SetOnAction(brick_action(down_right_brick));

	leftBreakAnimator->SetOnFinish(brick_finish(left_brick));
	rightBreakAnimator->SetOnFinish(brick_finish(right_brick));
	dleftBreakAnimator->SetOnFinish(brick_finish(down_left_brick));
	drightBreakAnimator->SetOnFinish(brick_finish(down_right_brick));

	leftBreakAnimator->Start(leftBreakAnimation, GETTIME);
	rightBreakAnimator->Start(rightBreakAnimation, GETTIME);
	dleftBreakAnimator->Start(dleftBreakAnimation, GETTIME);
	drightBreakAnimator->Start(drightBreakAnimation, GETTIME);

#undef BRICK_OFFSET
}

void Items::GenerateAEmptyBrickAnimation(Dim x, Dim y)
{
	std::string type(Q_BRICK_EMPTY);

	Sprite* emptyBrick = new Sprite(GETFILM(type), x, y, type);
	emptyBrick->SetBoundingArea(new BoundingBox(x, y, x + TILE_WIDTH, y + TILE_HEIGHT + 2));
	emptyBrick->SetHasDirectMotion(true);

	MovingAnimation* brickAnimation = new MovingAnimation(type, 0, 0, 0, 500);
	MovingAnimator* brickAnimator   = new MovingAnimator();
	
	this->animators[emptyBrick] = brickAnimator;

	brickAnimator->Start(brickAnimation, GETTIME);
}

void Items::GenerateFloatingCoin(Dim x, Dim y) 
{
	Sprite* powerUp = new Sprite(GETFILM(COIN_TYPE), x, y - TILE_HEIGHT, COIN_TYPE);
	powerUp->SetBoundingArea(new BoundingBox(x, y, x + TILE_WIDTH, y + TILE_HEIGHT));
	powerUp->SetHasDirectMotion(true);

	Path path;
	for (auto pos = 0; pos < 16; pos++)
		if (pos < 8) path.push_back(PathEntry(0, -4, 0, 40));
		else         path.push_back(PathEntry(0,  4, 0, 40));

	MovingPathAnimation* movingAnimation = new MovingPathAnimation(COIN_TYPE , path);

	MovingPathAnimator* movingAnimator = new MovingPathAnimator();

	movingAnimator->SetOnAction([powerUp](Animator* _animator, const Animation& _animation)
	{
		auto* movingAnimator = (MovingPathAnimator*)_animator;
		auto& movingAnimation = (MovingPathAnimation&)_animation;

		auto pathPos = movingAnimator->GetCurrPathPos();

		powerUp->Move(movingAnimation.GetPath()[pathPos].dx, movingAnimation.GetPath()[pathPos].dy);
	});

	movingAnimator->SetOnFinish([powerUp](Animator* _animator) 
	{
		SPRITE_MARK_AS_DEAD(powerUp);
		ANIMATOR_MARK_AS_DEAD(_animator);
	});

	movingAnimator->Start(movingAnimation, GETSYSTEMTIME);
}

//--------------------------------------------------INITIALIZE--------------------------------------------

void Items::MakeACoinAnimation(Dim x , Dim y)
{
	Sprite* coinFilm = new Sprite(GETFILM(COIN_TYPE), x, y, COIN_TYPE);
	coinFilm->SetBoundingArea(new BoundingBox(x, y, x + TILE_WIDTH, y + TILE_HEIGHT));
	coinFilm->SetHasDirectMotion(true);

	FrameRangeAnimation* coinAnimation = new FrameRangeAnimation(COIN_TYPE, 0 , 2 , 0 , 0 , 0 , 150);

	FrameRangeAnimator* coinAnimator = new FrameRangeAnimator();

	this->animators[coinFilm] = coinAnimator;

	coinAnimator->SetOnAction([coinFilm](Animator* animator, const Animation& animation) 
	{
		auto* frameAnimator = (FrameRangeAnimator*)animator;
		auto& frameAnimation = (FrameRangeAnimation&)animation;

		coinFilm->SetFrame(frameAnimator->GetCurrFrame());
	});

	coinAnimator->Start(coinAnimation, GETTIME);
}

void Items::MakeARBrickAnimation(Dim x , Dim y)
{
	Sprite* brickFilm = new Sprite(GETFILM(R_BRICK_TYPE) , x , y , R_BRICK_TYPE);
	brickFilm->SetBoundingArea(new BoundingBox(x, y, x + TILE_WIDTH, y + TILE_HEIGHT + 2));
	brickFilm->SetHasDirectMotion(true);

	Path path;
	path.push_back(PathEntry(0, 0, 0, 500));

	MovingPathAnimation* brickAnimation = new MovingPathAnimation(R_BRICK_TYPE , path , 0);

	MovingPathAnimator* brickAnimator = new MovingPathAnimator();

	this->animators[brickFilm] = brickAnimator;

	brickAnimator->SetOnAction([brickFilm](Animator* _animator, const Animation& _animation)
	{
		auto* pathAnimator = (MovingPathAnimator*)_animator;
		const auto& pathAnimation = (MovingPathAnimation&)_animation;

		auto pathPos = pathAnimator->GetCurrPathPos();
		auto path    = pathAnimation.GetPath();

		brickFilm->Move(path[pathPos].dx, path[pathPos].dy);
		brickFilm->SetBoundingAreaFromFrameBox();
	});

	brickAnimator->Start(brickAnimation, GETTIME);
}

void Items::MakeAQBrickAnimation(Dim x, Dim y)
{
	Sprite* brickFilm = new Sprite(GETFILM(Q_BRICK_TYPE), x, y, Q_BRICK_TYPE);
	brickFilm->SetBoundingArea(new BoundingBox(x, y, x + TILE_WIDTH, y + TILE_HEIGHT + 2));
	brickFilm->SetHasDirectMotion(true);

	Path paths;

	//Add a flasing animation to the question bricks
	//Epeleksa to path animation gia na mporesw poio meta na kanw to bounce
	paths.push_back(PathEntry(0 , 0 , 0 , 300));
	paths.push_back(PathEntry(0 , 0 , 1 , 300));
	paths.push_back(PathEntry(0 , 0 , 2 , 300));

	MovingPathAnimation* brickAnimation = new MovingPathAnimation(Q_BRICK_TYPE, paths , 0);

	MovingPathAnimator* brickAnimator = new MovingPathAnimator();

	this->animators[brickFilm] = brickAnimator;

	brickAnimator->SetOnAction([brickFilm](Animator* animator, const Animation& animation) 
	{
		auto* pathAnimator			= (MovingPathAnimator*)animator;
		const auto& pathAnimation	= (MovingPathAnimation&)animation;

		auto pathPos = pathAnimator->GetCurrPathPos();
		auto path	 = pathAnimation.GetPath();

		brickFilm->Move(path[pathPos].dx, path[pathPos].dy);
		brickFilm->SetFrame(path[pathPos].frame);
		brickFilm->SetBoundingAreaFromFrameBox();
	});

	brickAnimator->Start(brickAnimation, GETTIME);
}

void Items::MakeAPipeSprite(Dim x, Dim y, std::string& type)
{
	if (type == std::string("none"))
		return;

	Sprite* pipe = new Sprite(GETFILM(type), x, y, type);
	pipe->SetBoundingArea(new BoundingBox(x, y, x + TILE_WIDTH, y + TILE_HEIGHT));
	pipe->SetZorder(4);

	if( type == std::string("pipe_up_left")     || 
	    type == std::string("pipe_right_right") ||
		type == std::string("pipe_left_left"))
	{
		this->pipesSprites[pipe] = true;
	}
	else 
	{
		this->pipesSprites[pipe] = false;
	}
}

void Items::InitializeIndexes()
{
	//Tile set indexes for the question mark bricks
	this->qbricksIndexes.push_back(MakeIndex(12, 0));
	this->qbricksIndexes.push_back(MakeIndex(12, 1));
	this->qbricksIndexes.push_back(MakeIndex(12, 2));

	//Tile set indexes for the regular bricks
	this->rbricksIndexes.push_back(MakeIndex(2, 10));
	//this->rbricksIndexes.push_back(MakeIndex(2,11));

	//Tile set indexes for the coins
	this->coinsIndexes.push_back(MakeIndex(12, 3));
	this->coinsIndexes.push_back(MakeIndex(12, 4));
	this->coinsIndexes.push_back(MakeIndex(12, 5));

	//Tile set index for pipes and the type of each one
	this->pipes[MakeIndex(2, 3)] = std::string("pipe_up_left");
	this->pipes[MakeIndex(2, 4)] = std::string("none");
	this->pipes[MakeIndex(3, 3)] = std::string("pipe_up_down_left");
	this->pipes[MakeIndex(3, 4)] = std::string("pipe_up_down_right");

	this->pipes[MakeIndex(4, 1)] = std::string("pipe_right_left");
	this->pipes[MakeIndex(4, 2)] = std::string("pipe_right_right");
	this->pipes[MakeIndex(5, 1)] = std::string("pipe_right_down_left");
	this->pipes[MakeIndex(5, 2)] = std::string("none");

	this->pipes[MakeIndex(4, 5)] = std::string("pipe_left_left");
	this->pipes[MakeIndex(4, 6)] = std::string("pipe_left_right");
	this->pipes[MakeIndex(5, 5)] = std::string("none");
	this->pipes[MakeIndex(5, 6)] = std::string("pipe_left_down_right");
}

void Items::InitializeBricks()
{
	TileLayer& map = Terrain::GetActionLayer();

	auto sceneX = map.GetScene()[1].tileX * TILE_WIDTH;

	for (Dim row = 0; row < map.GetMapHeight(); ++row)
		for (Dim col = 0; col < map.GetMapWidth(); ++col)
		{
			Index tile = map.GetTile(row, col);

			auto rbrick_exist	= std::find(this->rbricksIndexes.begin(), this->rbricksIndexes.end(), tile);
			auto qbrick_exist	= std::find(this->qbricksIndexes.begin(), this->qbricksIndexes.end(), tile);
			auto coin_exist		= std::find(this->coinsIndexes.begin(), this->coinsIndexes.end(), tile);
			auto pipe_exist		= std::find_if(this->pipes.begin(), this->pipes.end(), [tile](std::pair<Dim, std::string> _pair) 
			{
				if (tile == _pair.first)
					return true;
				return false;
			});

			auto pair = std::make_pair(col * TILE_WIDTH, row * TILE_HEIGHT);

			if (rbrick_exist != this->rbricksIndexes.end())
			{
				this->MakeARBrickAnimation(pair.first, pair.second);
				map.SetTile(row, col, BACKGROUND_TILE);
			}
			else if (qbrick_exist != this->qbricksIndexes.end())
			{
				this->MakeAQBrickAnimation(pair.first, pair.second);
				map.SetTile(row, col, BACKGROUND_TILE);
			}
			else if (coin_exist != this->coinsIndexes.end())
			{
				this->MakeACoinAnimation(pair.first, pair.second);
				if(pair.first > sceneX)
					map.SetTile(row, col, BLACK_BACKGROUND);
				else
					map.SetTile(row, col, BACKGROUND_TILE);
			}
			else if (pipe_exist != this->pipes.end()) 
			{
				this->MakeAPipeSprite(pair.first, pair.second, pipe_exist->second);
				//map.SetTile(row, col, BACKGROUND_TILE);
			}
		}
}

void Items::InitializePowerUps()
{
	this->ReadPowerUps();
}

void Items::Initialize()
{
	this->InitializeIndexes();

	this->InitializeBricks();

	this->InitializePowerUps();
}

//--------------------------------------------------VIEW-WINDOW--------------------------------------------

void Items::DestroyItemsUsingViewWindow()
{
	Rect viewWindow = Terrain::GetActionLayer().GetViewWindow();

	for (ItemAnimators::iterator powerup = this->powerUpsAnimators.begin(); powerup != this->powerUpsAnimators.end();)
	{
		auto powerUpPosX = powerup->first->GetPositionX();
		auto powerUpPosY = powerup->first->GetPositionY();

		if (powerUpPosX <= viewWindow.x					|| 
			powerUpPosX >= viewWindow.x + viewWindow.w	|| 
			powerUpPosY >= 14 * TILE_HEIGHT)
		{
			SPRITE_MARK_AS_DEAD(powerup->first);
			ANIMATOR_MARK_AS_DEAD(powerup->second);

			powerup = this->powerUpsAnimators.erase(powerup);
		}
		else
			powerup++;
	}
}

void Items::SuspendUsingViewWindow()
{
	auto scene = Terrain::GetActionLayer().GetCurrentScene();
	auto scenes = Terrain::GetActionLayer().GetScene();

	auto viewWindow = Terrain::GetActionLayer().GetViewWindow();

	for (auto item : this->animators)
	{
		if (item.first->GetPositionX() < viewWindow.x || (item.first->GetPositionX() > (viewWindow.x + viewWindow.w)))
		{
			item.first->SetVisibility(false);
			SUSPEND(item.second);
		}
		else
		{
			item.first->SetVisibility(true);
			RUN(item.second);
		}
	}

	for (auto pipe : this->pipesSprites) 
	{
		if (pipe.first->GetPositionX() < scenes[scene].tileX * TILE_WIDTH)
			pipe.first->SetVisibility(false);
		else
			pipe.first->SetVisibility(true);
	}

}

void Items::CheckForDead()
{
	for (ItemAnimators::iterator animator = this->animators.begin(); animator != this->animators.end();)
	{
		if (animator->first->GetStateId() == DEAD_STATE)
		{
			animator->first->SetVisibility(false);
			SPRITE_MARK_AS_DEAD(animator->first);
			ANIMATOR_MARK_AS_DEAD(animator->second);

			animator = this->animators.erase(animator);
		}
		else
			animator++;
	}
}

void Items::ItemActions()
{
	this->DestroyItemsUsingViewWindow();

	this->CheckForDead();

	if(Terrain::GetActionLayer().GetDisplayChanged())
		this->SuspendUsingViewWindow();
}

//--------------------------------------------------SETERS/GETERS--------------------------------------------

void Items::UpdateRunningAnimatorsTime(Timestamp _now)
{
	for (auto anim : this->animators)
		anim.second->SetLastTime(_now);
}

ItemAnimators& Items::GetAnimators()
{
	return this->animators;
}

ItemAnimators& Items::GetPowerUpAnimators()
{
	return this->powerUpsAnimators;
}

Pipes& Items::GetPipes()
{
	return this->pipesSprites;
}

Items& Items::GetSigleton()
{
	return items;
}
