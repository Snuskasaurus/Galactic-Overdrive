#ifndef ENEMY__H
#define ENEMY__H

// -- INCLUDES ------------------------------------------------------------- //
#include "_Engine.h"

// Components
#include "Entity.h"
#include "Collider.h"
#include "Timer.h"

// Others Entities
#include "Projectiles.h"
#include "Weapon.h"
#include "Turret.h"
#include "Path.h"
#include "Explosion.h"
#include "Laser.h"
#include "Score.h"
#include "Money.h"

// -- FUNCTIONS ------------------------------------------------------------ //

Enemies *Enemy_Initialize();

//Must init weapon and turret blueprint before
BP_Enemies EnemyLoader(Engine *_engine);

Enemy *GetCopyOfEnemyBlueprint(Blueprint *_blueprints, const char *_enemyName);

Enemy *Enemy_Create(Engine *_engine, Enemies *_enemies, Waypoint *_spawn, Player *_target, Difficulty *_currentDif);

void Enemy_Update(Engine *_engine, Enemies *_enemies, float _dt, List *_projectiles, List* _explosions, Lasers *_lasers, Money** _money);

void Enemy_Display(Engine *_engine, Enemies *_enemies);

void Enemy_TakeDamage(Engine *_engine, MainData *_data, Weapon *_weaponUsed, int _index, sfVector2f* _position);

void Enemy_Destroy(Enemies *_enemies, int _index, Lasers *_lasers);

void Enemy_DestroyAll(Enemies *_enemies, Lasers *_lasers);

#endif // !PLAYER__H