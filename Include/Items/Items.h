#ifndef ITEMS_H
#define ITEMS_H

#include "AnimationFilmHolder.h"
#include "AnimatorManager.h"
#include "EnemiesManager.h"
#include "FrameRangeAnimator.h"
#include "MovingAnimator.h"
#include "MovingPathAnimator.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "SoundsManager.h"
#include "SystemTime.h"
#include "Terrain.h"
#include "Utilities.h"
#include "Terrain.h"

#include <vector> 
#include <map>

#define BACKGROUND_TILE		MakeIndex(2,9)
#define BLACK_BACKGROUND	MakeIndex(2,11)
#define EMPTY_BRICK_TILE	MakeIndex(0,11)
#define SUPER_MUSHROOM_TILE MakeIndex(12,6)
#define ONE_UP_TILE			MakeIndex(12,7)
#define STARMAN_TILE		MakeIndex(12,8)
#define COIN_TILE			MakeIndex(12,3)

#define POWER_UPS_PATH		"../Media/Terrain/power-ups.csv"

#define R_BRICK_TYPE		"rbrick"
#define R_BRICK_BREAK       "rbrick_break"
#define Q_BRICK_TYPE		"qbrick"
#define Q_BRICK_EMPTY		"qbrick_empty"

#define COIN_TYPE			"coin"
#define	MUSHROOM_TYPE		"super_mushroom" 
#define	ONE_UP_TYPE			"1_up"
#define STARMAN_TYPE		"starman"
#define NONE_TYPE			"none"

#define DEAD_STATE			"Dead"

using ItemAnimators		= std::map<Sprite*, Animator*>;
using PowerUps			= std::map<std::pair<Sprite* , std::string>, bool >;
using PowerUpPair		= std::pair<Sprite*, MovingAnimator*>;

using PipeHolder		= std::map<Dim,std::string>;
using Pipes				= std::map<Sprite* , bool>;
using ItemsIndexes		= std::vector<Dim>;


class Items
{
  private:

	  ItemAnimators animators;
	  ItemAnimators powerUpsAnimators;

	  ItemsIndexes rbricksIndexes;
	  ItemsIndexes qbricksIndexes;
	  ItemsIndexes coinsIndexes;

	  PowerUps	    powerUps;

	  PipeHolder    pipes;
	  Pipes			pipesSprites;

	  static Items items;

	  bool firstTimeLoad = true;

  private:
	  
	  void InitializeIndexes();
	  void InitializePowerUps();
	  void InitializeBricks();

	  void MakeACoinAnimation(Dim x , Dim y);
	  void MakeARBrickAnimation(Dim x , Dim y);
	  void MakeAQBrickAnimation(Dim x , Dim y);
	  void MakeAPipeSprite(Dim x, Dim y , std::string& type);

	  void ReadPowerUps();

	  void DestroyItemsUsingViewWindow();
	  void SuspendUsingViewWindow();
	  void CheckForDead();

  public:

	  void GenerateABounceBrickAnimation(MovingPathAnimator* _animator);
	  void GenerateABrickBreakAnimation(Dim x, Dim y);
	  void GenerateAEmptyBrickAnimation(Dim x, Dim y);
	  PowerUpPair GeneratePowerUp(Dim x , Dim y , std::string _type);
	  void GenerateFloatingCoin(Dim x, Dim y);

	  std::string ActivatePowerUp(Dim x, Dim y);
	  int IsPowerUpActive(Dim x, Dim y);

	  void UpdateRunningAnimatorsTime(Timestamp _now);

	  ItemAnimators& GetAnimators();
	  ItemAnimators& GetPowerUpAnimators();
	  Pipes& GetPipes();

	  void Initialize();

	  void ItemActions();

	  static Items& GetSigleton();
};

#endif // !ITEMS_H

