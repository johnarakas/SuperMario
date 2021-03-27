#include "App.h"
#include "Terrain.h"
#include "Items.h"
#include "AnimatorManager.h"
#include "EnemiesManager.h"
#include "SpriteManager.h"
#include "Mario.h"
#include "SoundsManager.h"
#include "CollisionChecker.h"
#include "ScoreLayer.h"

class MarioGame : public app::App 
{	
	public:

		MarioGame() = default;
		
		void Initialise(void)	override;
		void Load(void)			override;
		void Clear(void)		override;
};

bool InitializeAllegro();

bool InitializeMarioGame();

void MakeGameOverWait(app::Game& game);

