#include "Piranha.h"
#include "Mario.h"

EnemiesTempate<Piranha> piranhasManager;

const CollisionAction Piranha::MakeCollisionAction() 
{
	return ([this](Sprite* s1, Sprite* s2)
	{
		Mario& mario = Mario::GetMario();
		
		if (!s1->GetVisibility())
			return;

		if (mario.GetMarioSprite() != s2)
			return;

		if (this->GetState() == PIRANHA_DEAD)
			return;

		auto marioState = mario.GetMarioState();

		if (marioState == MARIO_DYING    ||
			marioState == MARIO_DEAD)
			return;

		if (ScoreLayer::GetLives() <= 0)
			return;

		auto& piranhas = piranhasManager.GetAliveEnemies();

		auto marioPosX = s2->GetPositionX();
		auto marioPosY = s2->GetPositionY();

		auto marioType = mario.GetMarioType();

		for (auto piranha : piranhas) 
		{
			if (piranha->GetState() == PIRANHA_DEAD)
				continue;
			
			auto piranhaPosX = piranha->GetSprite()->GetPositionX();
			auto piranhaPosY = piranha->GetSprite()->GetPositionY();

			auto collideCorrectX = (piranhaPosX >= marioPosX) ? piranhaPosX - marioPosX : marioPosX - piranhaPosX;
			auto isThatPiranhaSide = collideCorrectX <= 14;

			auto collideCorrectY = (piranhaPosY >= marioPosY) ? piranhaPosY - marioPosY : marioPosY - piranhaPosY;
			auto isThatPiranhaFromUp = collideCorrectY <= 14;

			if (isThatPiranhaSide && isThatPiranhaFromUp) 
			{
				switch (marioType) 
				{
					case SMALL_INVINCIBLE_MARIO:
					case SUPER_INVINCIBLE_MARIO:
					{
						piranha->SetState(PIRANHA_DEAD);
						ADD_SCORE(200);
						break;
					}
					case SMALL_MARIO:
					{
						REMOVE_LIVE(1);

						PLAYSOUND("mario_die");
						mario.MarioDie();
						break;
					}
					case SUPER_MARIO:
					{
						mario.SetMarioType(SMALL_MARIO);
						break;
					}
				}
				return;
			}
		}
	});
}

void Piranha::CreatePiranha(Dim x, Dim y)
{
	GridLayer* grid = Terrain::GetActionLayer().GetGridLayer();

	std::string type(PIRANHA_TYPE);

	Dim x1 = x + PIRANHA_X_OFFSET;
	Dim y1 = y + PIRANHA_Y_OFFSET;
	Dim x2 = x + PIRANHA_X_OFFSET + TILE_WIDTH;
	Dim y2 = y + PIRANHA_Y_OFFSET + TILE_HEIGHT + PIRANHA_Y_PLUS;

	this->piranhaFilm = new Sprite(GETFILM(type), x1, y1, type);
	this->piranhaFilm->SetBoundingArea(new BoundingBox(x1  , y1 , x2 , y2));
	this->piranhaFilm->SetHasDirectMotion(true);

	Path path;
	//The first path entry is for the delay
	path.push_back(PathEntry(0, 0, 0, 3000));
	for (auto pos = 0; pos < 12; pos++)
	{
		auto _frame = pos % 2;

		if (pos < 6)	path.push_back(PathEntry(0, -4, _frame, 120));
		else			path.push_back(PathEntry(0, 4, _frame, 120));
	}

	MovingPathAnimation* goombaAnimation = new MovingPathAnimation(type, path , 0);

	this->piranhaAnimator = new MovingPathAnimator();

	this->piranhaAnimator->SetOnAction([this](Animator* _animator, const Animation& _animation)
	{
		auto* pathAnimator = (MovingPathAnimator*)_animator;
		auto& pathAnimation = (MovingPathAnimation&)_animation;

		auto pathPos = pathAnimator->GetCurrPathPos();
		auto path	 = pathAnimator->GetAnim()->GetPath();

		/*if (pathPos == 1)
			this->SetState(PIRANHA_UP);
		else if(pathPos == 11)
			this->SetState(PIRANHA_DOWN);*/

		this->piranhaFilm->Move(path[pathPos].dx, path[pathPos].dy);
		this->piranhaFilm->SetFrame(pathAnimation.GetPath()[pathPos].frame);
		this->piranhaFilm->SetBoundingAreaFromFrameBox();
	});

	for (auto* marioSprite : Mario::GetMario().GetMarioSprites())
		REGISTER(this->piranhaFilm, marioSprite, this->MakeCollisionAction());

	this->piranhaAnimator->Start(goombaAnimation, GETTIME);
}

Piranha::Piranha(Dim _x, Dim _y)
{
	this->CreatePiranha(_x, _y);

	piranhasManager.Register(this);
}

Sprite* Piranha::GetSprite()
{
	return this->piranhaFilm;
}

MovingPathAnimator* Piranha::GetAnimator()
{
	return this->piranhaAnimator;
}

PiranhaState Piranha::GetState()
{
	return this->piranhaState;
}

void Piranha::SetState(PiranhaState _state)
{
	this->piranhaState = _state;
}

bool Piranha::IsDead() const
{
	return this->piranhaState == PIRANHA_DEAD;
}
