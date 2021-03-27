#include "Main.h"

bool InitializeAllegro()
{
	if (!al_init())
	{
		std::cerr << "Failed to initialize Allegro!\n";
		return false;
	}

	if (!al_init_image_addon())
	{
		std::cerr << "Failed to initialize Image Addon!\n";
		return false;
	}

	if (!al_init_primitives_addon())
	{
		std::cerr << "Failed to initialize Primitive Addon!\n";
		return false;
	}

	if (!al_init_font_addon())
	{
		std::cerr << "Failed to initialize Font Addon!\n";
		return false;
	}

	if (!al_install_audio())
	{
		std::cerr << "Failed to install audio.\n";
		return false;
	}

	if (!al_init_acodec_addon())
	{
		std::cerr << "Failed to initialize Audio Addon!\n";
		return false;
	}

	if (!al_init_ttf_addon())
	{
		std::cerr << "Failed to initialize ttf Addon!\n";
		return false;
	}

	if (!(display = al_create_display(MAX_SCREEN_WIDTH, MAX_SCREEN_HEIGHT)))
	{
		std::cerr << "Failed to create the main Display\n";
		return false;
	}

	if (!al_install_keyboard())
	{
		std::cerr << "Failed to install the Keyboard!\n";
		al_destroy_display(display);
		return false;
	}

	if (!(queue = al_create_event_queue()))
	{
		std::cerr << "Failed to create the event queue!\n";
		al_destroy_display(display);
		return false;
	}

	if (!(timer = al_create_timer(1.0 / 120)))
	{
		std::cerr << "Failed to create the timer!\n";
		al_destroy_event_queue(queue);
		al_destroy_display(display);
		return false;
	}

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_keyboard_event_source());

	al_start_timer(timer);

	return true;
}

bool InitializeMarioGame()
{
	Timestamp  start;
	Timestamp  end;

	if (!ScoreLayer::GetSigleton().InitializeScoreLayer())
	{
		al_destroy_event_queue(queue);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return false;
	}

	start = GETTIME;
	Terrain::CreateSigleton();
	end = GETTIME;
	std::cout << "Terrain Create: [ " + std::to_string((end - start)) + " ms ]\n";

	start = GETTIME;
	AnimationFilmHolder::GetSigleton().LoadAll(std::string("../Config/enemies_animations.config"));
	AnimationFilmHolder::GetSigleton().LoadAll(std::string("../Config/items_animations.config"));
	AnimationFilmHolder::GetSigleton().LoadAll(std::string("../Config/mario_animations.config"));
	end = GETTIME;
	std::cout << "AnimationFilmHolder Load: [ " + std::to_string((end - start)) + " ms ]\n";

	start = GETTIME;
	Items::GetSigleton().Initialize();
	end = GETTIME;
	std::cout << "Items Create: [ " + std::to_string((end - start)) + " ms ]\n";

	start = GETTIME;
	SoundsManager::GetSigleton().LoadAllSounds(std::string("../Config/mario_sounds.config"));
	end = GETTIME;
	std::cout << "Sounds Load: [ " + std::to_string((end - start)) + " ms ]\n";

	start = GETTIME;
	Mario::GetMario().InitializeMario();
	end = GETTIME;
	std::cout << "Mario Create: [ " + std::to_string((end - start)) + " ms ]\n";

	start = GETTIME;
	EnemiesManager::GetSigleton().InitializeEnemies();
	end = GETTIME;
	std::cout << "Enemies Create: [ " + std::to_string((end - start)) + " ms ]\n";

	SETSYSTEMTIME;

	Items::GetSigleton().UpdateRunningAnimatorsTime(GETTIME);

	AnimatorManager::GetSigleton().Pause();

	return true;
}

void MakeGameOverWait(app::Game& game) 
{
	PLAYSOUND("game_over");

	TickAnimation* gameOverWait = new TickAnimation("game_over", 5000, 1, true);

	TickAnimator* tickAnimator = new TickAnimator();

	tickAnimator->SetOnFinish([&game](Animator* anim)
	{
		game.SetIsFinished(true);
	});

	tickAnimator->Start(*gameOverWait, GETTIME);
}

void MarioGame::Initialise(void)
{
	if (InitializeAllegro())
	{
		if (!InitializeMarioGame())
		{
			std::cout << "Error to load Mario Game.\n";
		}
	}
	else
		std::cout << "Error to load Allegro 5.\n";
}

void MarioGame::Load(void)
{	
	GetGame().startTime = GETTIME;

	GetGame().SetOnStartGame([this]() 
	{
		if (this->GetGame().isStart)
		{
			AnimatorManager::GetSigleton().Resume();
			AnimatorManager::GetSigleton().TimeShift(GETTIME - this->GetGame().startTime);
			
			this->GetGame().isStart   = false;
			this->GetGame().startTime = 0;
		}
	});
	
	GetGame().SetIsFinished([this]() 
	{
		return GetGame().GetIsFinished();
	});

	GetGame().SetOnPauseResume([this]() 
	{
		if (!GetGame().IsPaused()) 
		{
			AnimatorManager::GetSigleton().Resume();
			AnimatorManager::GetSigleton().TimeShift(GETTIME - this->GetGame().GetPauseTime());
		}
	});

	GetGame().SetRender([this]() 
	{
		Bitmap at				= al_get_backbuffer(display);
		TileLayer& actionLayer	= Terrain::GetActionLayer();
		Mario& mario			= Mario::GetMario();

		al_clear_to_color(al_map_rgba(0, 0, 0, 0));
		
		if (game.isStart) 
		{
			Terrain::Display(at, GetGame().IsPaused() , false , true);
		}
		
		mario.MakeMarioDie();

		if (!mario.IsDead() && !game.isStart)
		{
			Terrain::Display(at, GetGame().IsPaused() , false , false);

			ScoreLayer::GetSigleton().Draw();
			
			mario.Draw(at);

			SpriteManager::GetSigleton().Draw(at);

			if (actionLayer.GetGridLayer()->GetDisplayGrid())
			{
				actionLayer.GetGridLayer()->DisplayGrid(at, actionLayer.GetViewWindow(), [](Bitmap bitmap, int x, int y, int w, int h) {
					al_draw_rectangle(x, y, x + w, y + h, al_map_rgb(255, 0, 0), 0);
				});
			}

			actionLayer.SetDisplayChanged(false);
		}
		else if(!game.isStart)
		{
			Terrain::Display(at, false, true, false);
			
			MakeGameOverWait(GetGame());
		}

		if (event.type == ALLEGRO_EVENT_TIMER && al_is_event_queue_empty(queue))
		{
			al_flip_display();
		}

	});

	GetGame().SetInput([this]() 
	{
		TileLayer& tileLayer			= Terrain::GetActionLayer();
		CircularBackground* background	= Terrain::GetBackground();
		Mario& mario					= Mario::GetMario();
		app::Game& game					= GetGame();

		al_wait_for_event(queue, &event);
		al_get_keyboard_state(&state);

		if(!game.IsPaused() && mario.GetMarioState() != MARIO_ENTER_PIPE && !GetGame().isStart)
		{
			if (al_key_down(&state, ALLEGRO_KEY_D)) 
			{
				if (al_key_down(&state, ALLEGRO_KEY_LSHIFT))
					mario.MarioRunRight();
				else 
					mario.MarioWalkRight();
			}

			if (al_key_down(&state, ALLEGRO_KEY_A)) 
			{
				if (al_key_down(&state, ALLEGRO_KEY_LSHIFT))
					mario.MarioRunLeft();
				else
					mario.MarioWalkLeft();
			}

			/*if (al_key_down(&state, ALLEGRO_KEY_W)) 
			{
				mario.MarioStandingAndJumping();
			}*/
		}

		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_W && !GetGame().isStart && !game.IsPaused())
			{
				mario.MarioStandingAndJumping();
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				game.SetIsFinished(true);
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
			{
				game.StartGame();
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_P && 
				!GetGame().isStart						&& 
				!mario.IsDead())
			{
				if (game.IsPaused())
				{
					if (mario.IsDying())
						PLAYSOUND("mario_die");
					game.Resume();
				}
				else
				{
					if (mario.IsDying())
						STOPSOUND("mario_die");

					AnimatorManager::GetSigleton().Pause();

					game.Pause(GETSYSTEMTIME);
				}

				PLAYSOUND("pause");
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_DOWN && 
				mario.IsMarioOnPipe() && 
				mario.GetMarioState() != MARIO_ENTER_PIPE)
			{
				mario.MarioEnterPipe();
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT && 
				mario.CheckMarioOnSecondPipe() && 
				mario.GetMarioState() != MARIO_ENTER_PIPE)
			{
				mario.MarioExitPipe();
			}
			
			if (event.keyboard.keycode == ALLEGRO_KEY_G)
			{
				tileLayer.GetGridLayer()->SetDisplayGrid(!tileLayer.GetGridLayer()->GetDisplayGrid());
			}
		}
	});

	GetGame().SetProgressAnimations([]() 
	{	
		AnimatorManager::GetSigleton().Progress(GETSYSTEMTIME);
	});

	GetGame().SetAI([]() 
	{
		Items::GetSigleton().ItemActions();
	
		EnemiesManager::GetSigleton().EnemiesActions();
	});

	GetGame().SetCollisionChecking([]() 
	{
		CollisionChecker::GetSigleton().Check();
	});

	GetGame().SetCommitDestructions([]() 
	{
		EnemiesManager::GetSigleton().CommitDestruction();

		SpriteManager::GetSigleton().CommitDestruction();
		AnimatorManager::GetSigleton().CommitDestruction();
	});
}

void MarioGame::Clear(void)
{
	//Clean Up game stuff
	BitmapLoader::GetSigleton().Destroy();
	ScoreLayer::GetSigleton().Destroy();

	//Clean allegro stuff
	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_destroy_display(display);
}

int main() 
{
	app::App* app = new MarioGame();

	app->Main();
}
