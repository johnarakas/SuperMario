#ifndef ENEMIES_MANAGER_H
#define ENEMIES_MANAGER_H

#include <map>
#include <utility>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "Utilities.h"
#include "Goomba.h"
#include "Piranha.h"
#include "KoopaTroopa.h"

#define ENEMIES_PATH "../Media/Terrain/enemies.csv"

enum EnemieType 
{
	GOOMBA,
	GREEN_KOOPA,
	RED_KOOPA,
	PIRANHA
};

using EnemiesPos = std::map<std::pair<Dim, Dim>, EnemieType>;

class EnemiesManager 
{
	private:
		
		static EnemiesManager manager;

		EnemiesPos enemies;

	private:

		EnemiesManager()  = default;
		~EnemiesManager() = default;

		void ReadEnemiesMap();

		void CreateEnemies();

	public:

		void InitializeEnemies();

		void CommitDestruction();

		void EnemiesActions();

		EnemiesPos& GetEnemies();

		static EnemiesManager& GetSigleton();
};

#endif // !ENEMIES_MANAGER_H


