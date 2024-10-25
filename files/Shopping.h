#ifndef SHOPPING_H
#define SHOPPING_H

// -- INCLUDES --------------------------------------------------------------- //

#include "_Engine.h"

// Entities
#include "Weapon.h"

// -- SHARED PROTOTYPES ------------------------------------------------------ //

void Shopping_Update(GameData *_state, Engine *_engine, float _dt);

void Shopping_Display(GameData *_state, Engine *_engine);

Weapon Shopping_BuyWeapon(BP_Weapons *_weaponsBP, const char *_weaponName, Player *_player, GameData *_state, Weapon *_weaponToBeUpgraded, Engine *_engine);

sfBool Shopping_BuySkillCap(Player *_player, GameData *_state, int _price);

sfBool Shopping_BuySkillCharge(Player *_player, GameData *_state, int _price);

#endif