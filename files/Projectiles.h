#ifndef PROJECTILES__H
#define PROJECTILES__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"

// Components
#include "Collider.h"
#include "Entity.h"

// -- FUNCTIONS ------------------------------------------------------------ //

void Projectile_Create(Engine *_engine, List *_projectiles, Weapon *_origin);

void Projectile_Update(Engine *_engine, float _dt, List *_projectiles, GameData *_state, GameState _gameState);

void Projectile_Display(Engine *_engine, List *_projectiles);

#endif // !PROJECTILES__H