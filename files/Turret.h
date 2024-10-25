#ifndef TURRET__H
#define TURRET__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_MainData.h"

// Components
#include "Collider.h"
#include "Entity.h"

// Others Entities
#include "Weapon.h"

// -- FUNCTIONS ------------------------------------------------------------ //

BP_Turrets Turret_LoadBlueprints();

Turret Turret_Create(Engine *_engine, const char *_name, WeaponOwner _ownerType, void *_ownerPointer, sfVector2f _offset);

void Turret_Update(Engine *_engine, Turret *_turret, List *_projectiles, sfVector2f _aimingPosition, sfVector2f _ownerPosition);

void Turret_Display(Engine *_engine, Turret *_turret, sfRenderStates *_renderState);

#endif // !TURRET__H