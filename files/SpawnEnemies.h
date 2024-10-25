#ifndef SPAWN_ENEMIES__H
#define SPAWN_ENEMIES__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Includes.h"
#include "_Engine.h"

#include "Enemy.h"
#include "Player.h"

// -- FUNCTIONS ------------------------------------------------------------ //

void Mechanic_SpawnEnemies(MainData *_data, Level *_level, Enemies *_enemies, Players *players, float _dt, Boss *_boss, Difficulty *_currentDif);

#endif //!SPAWN_ENEMIES__H
