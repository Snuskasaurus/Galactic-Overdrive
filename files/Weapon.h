#ifndef WEAPONS__H
#define WEAPONS__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"

// Components
#include "Entity.h"
#include "Projectiles.h"

// -- FUNCTIONS ------------------------------------------------------------ //

BP_Weapons Weapon_LoadBlueprints(Animations *_anims);

Weapon GetWeapon(const char _weaponName[], BP_Weapons *_blueprints, WeaponOwner _owner, void *_pointer);

Weapon* Weapon_GetPointer(const char _weaponName[], BP_Weapons *_blueprints);

void Weapon_UpdatePosition(Weapon *_weapon, sfVector2f _entityPosition, sfVector2f _offset);

void Weapon_SetPosition(Weapon *_weapon, sfVector2f _entityPosition, sfVector2f _offset);

void Weapon_UpdateFiringRate(Engine *_engine, Weapon *_weapon);

void Weapon_ShootProjectile(Engine *_engine, List *_projectiles, Weapon *_weapon);

void Weapon_UpdateOrientation(Weapon *_weapon, joystickId _joystick);

void Weapon_Update(Engine *_engine, Weapon *_weapon, List *_projectiles, Entity *_owner, Entity *_target);

#endif // !WEAPONS__H