#include "EnemiesManager.h"

EnemiesManager EnemiesManager::manager;

void EnemiesManager::ReadEnemiesMap()
{
	std::ifstream fileToRead;
	std::string line;

	Dim row = 0;
	Dim col = 0;

	Dim backgroundTile = 47;

	fileToRead.open(ENEMIES_PATH, std::ios::in);
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
			if (index != backgroundTile)
			{
				auto pair = std::make_pair(col * TILE_WIDTH, row * TILE_HEIGHT);

				switch (index) 
				{
					case 1 : enemies[pair] = GOOMBA;		break;
					case 2 : enemies[pair] = GREEN_KOOPA;	break;
					case 3 : enemies[pair] = RED_KOOPA;		break;
					case 4 : enemies[pair] = PIRANHA;		break;
					default: enemies[pair] = GOOMBA;		break;
				}
			}
			col++;
		}
		row++;
	}

	fileToRead.close();
}

void EnemiesManager::CreateEnemies()
{
	for (auto enemie : enemies) 
	{
		switch (enemie.second) 
		{
			case GOOMBA:
				new Goomba(enemie.first.first, enemie.first.second);
				break;
			case GREEN_KOOPA:
				new KoopaTroopa(GREEN, enemie.first.first, enemie.first.second);
				break;
			case RED_KOOPA:
				new KoopaTroopa(RED, enemie.first.first, enemie.first.second);
				break;
			case PIRANHA:
				new Piranha(enemie.first.first, enemie.first.second);
				break;
			default:
				assert(0);
		}
	}
}

void EnemiesManager::InitializeEnemies()
{
	ReadEnemiesMap();

	CreateEnemies();
}

void EnemiesManager::CommitDestruction()
{
	piranhasManager.CommitDestruction();
	goombaManager.CommitDestruction();
	koopaTroopaManager.CommitDestruction();
}

void EnemiesManager::EnemiesActions()
{
	piranhasManager.Actions();
	goombaManager.Actions();
	koopaTroopaManager.Actions();
}

EnemiesPos& EnemiesManager::GetEnemies()
{
	return enemies;
}

EnemiesManager& EnemiesManager::GetSigleton()
{
	return manager;
}
