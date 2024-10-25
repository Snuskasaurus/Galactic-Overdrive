#ifndef SHOP__H
#define SHOP__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_MainData.h"
#include "_Includes.h"
#include "Game.h"

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
#include "Fade.h"

// Mechanics
#include "SpawnEnemies.h"
#include "Shopping.h"

// -- ENUMERATIONS --------------------------------------------------------- //

// -- DEFINES -------------------------------------------------------------- //

// -- STRUCTURES ----------------------------------------------------------- //

// -- MAIN FUNCTION DEFINITIONS -------------------------------------------- //

void Shop_Initialize(MainData *_data);
void Shop_HandleEvent(MainData *_data, sfEvent _event);
void Shop_Update(MainData *_data, float _dt);
void Shop_Display(MainData *_data);
void Shop_Exit(MainData *_data);

#endif // !SHOP__H