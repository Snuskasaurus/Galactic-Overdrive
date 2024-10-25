#ifndef DIRECTIONNAL_SHIELD_H
#define DIRECTIONNAL_SHIELD_H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"

// Components
#include "Entity.h"
#include "Collider.h"
#include "Timer.h"

// Others Entities
#include "Projectiles.h"
#include "Weapon.h"

// -- FUNCTIONS ------------------------------------------------------------ //

int DirectionalShield_isColliding(Player* _player, ProjectileData* _projectile);
void DirectionalShield_AbsorbBullet(ProjectileData* _projectile, Weapon* _directionalShield, Engine* _engine);
void DirectionalShield_Regenlife(Weapon* _directionalShield, float _dt, Engine* _engine);
void DirectionalShield_Attack(Engine *_engine, List *_projectiles, Weapon *_directionalShield);
void DirectionalShield_ActivationOperation(Weapon *_weapon, joystickId _joystick, Engine* _engine);

#endif // !DIRECTIONNAL_SHIELD_H
