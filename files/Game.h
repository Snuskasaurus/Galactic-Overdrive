#ifndef GAME__H
#define GAME__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_MainData.h"
#include "_Includes.h"

// Entities
#include "Player.h"
#include "Weapon.h"
#include "Projectiles.h"
#include "Enemy.h"
#include "Map.h"
#include "Path.h"
#include "Level.h"
#include "World.h"
#include "DirectionalShield.h"
#include "Laser.h"
#include "Money.h"
#include "Explosion.h"
#include "Boss.h"
#include "Score.h"
#include "Difficulty.h"
#include "AnimatedStarsBackground.h"
#include "Shadow.h"
#include "Achievements.h"
#include "Fade.h"
#include "Progression.h"

// Mechanics
#include "SpawnEnemies.h"

// -- ENUMERATIONS --------------------------------------------------------- //

// -- DEFINES -------------------------------------------------------------- //

// -- STRUCTURES ----------------------------------------------------------- //

// -- MAIN FUNCTION DEFINITIONS -------------------------------------------- //

void Game_Initialize(MainData *_data);
void Game_HandleEvent(MainData *_data, sfEvent _event);
void Game_Update(MainData *_data, float _dt);
void Game_HandleBossCollisions(MainData *_data);
void Game_HandlePlayersCollisions(MainData *_data);
void Game_HandleEnemiesCollisions(MainData *_data, sfVector2f* _position);
void Game_HandleProjectilesCollisions(MainData *_data);
void Game_HandleLasersCollisions(MainData *_data, float _dt, sfVector2f* _position);
void Game_Display(MainData *_data);
void Game_Exit(MainData *_data);

#endif // !GAME__H