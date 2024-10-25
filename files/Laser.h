#ifndef LASER_H
#define LASER_H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"

// Components
#include "Entity.h"

// Others Entities
#include "Collider.h"

// -- FUNCTIONS ------------------------------------------------------------ //

void Laser_Create(Engine *_engine, Lasers *_lasers, Weapon *_origin, void *_entity);

Lasers *Laser_InitLinkedList();

void Laser_DeleteFirst(Lasers *_list, WeaponOwner _owner);

Laser *Laser_Delete(Lasers *_list, Laser *_element, Laser *_elementToDelete, WeaponOwner _owner);

void Laser_DestroyList(Lasers *_list);

sfBool Laser_IsAlreadyShooting(Lasers *_list, void *_entity, Weapon *_weapon);

void Laser_Update(Lasers *_list, Engine* _engine);

void Laser_StopShooting(Lasers *_lasers, void *_entity, Weapon *_weapon);

void Laser_Display(Engine *_engine, Lasers *_list);

#endif