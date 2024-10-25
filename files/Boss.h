#ifndef BOSS__H
#define BOSS__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"

// Components
#include "Entity.h"
#include "Collider.h"
#include "Timer.h"

// Others Entities
#include "Projectiles.h"
#include "Weapon.h"
#include "Laser.h"
#include "Turret.h"
#include "Enemy.h"
#include "Path.h"

// -- FUNCTIONS ------------------------------------------------------------ //

BP_Boss Boss_InitializeBlueprints(Engine * _engine);

Boss Boss_Create(Engine *_engine, const char *_name);

void Boss_Update(Engine* _engine, Boss *_boss, List *_projectiles, Lasers *_lasers, sfMusic ** _levelMusic, Enemies *_enemies, Paths *_paths);

void Boss_Display(Engine *_engine, Boss *_boss);

float Boss_CalculateDamage(Engine * _engine, Weapon * _weaponUsed);

void Boss_TakeDamageFromWeakpoint(Engine * _engine, Boss *_boss, int _weakpointID, float _damage);

void Boss_TakeDamageFromTurret(Engine * _engine, Boss *_boss, int _turretID, float _damage);

void Boss_TakeDamage(Engine * _engine, Boss *_boss, Weapon * _weaponUsed);

void Boss_UpdateTurret(Engine *_engine, Boss *_boss, Players *_players, List *_projectiles);

#endif // !BOSS__H