#ifndef PLAYER__H
#define PLAYER__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"
#include "_MainData.h"

// Components
#include "Collider.h"
#include "Entity.h"

// Others Entities
#include "Projectiles.h"
#include "Weapon.h"
#include "Laser.h"
#include "DirectionalShield.h"

#include "Skill.h"

// -- FUNCTIONS ------------------------------------------------------------ //

BP_Players Player_LoadBlueprints(Engine *_engine);

Player *Player_Create(Engine *_engine, Players *_players, sfVector2f _position, joystickId _joystick, const char *_blueprintnNme);

void Player_JoystickButtonPressed(joystickId _joystick, joystickButton _button, GameData *_state, GameState _currentState, Engine *_engine);

void Player_JoystickButtonReleased(joystickId _joystick, joystickButton _button, GameData *_state);

void Player_JoystickMoved(MainData *_data, joystickId _joystick, sfJoystickAxis _axis, float _position);

void Player_HandleMovementInput(Players *_players);
void Player_HandleShopMovementInput(Players *_players);

void HandleShootInput(Engine *_engine, Players *_players, List *_projectiles, Lasers *_lasers);
void Player_ShopTestWeapon(Engine *_engine, Players *_players, List *_projectiles, Lasers *_lasers, Weapon* _testWeapon[MAX_PLAYER_NUMBER]);

void Player_TakeDamage(MainData* _data, Player *_player, Weapon *_weaponUsed);

void Player_Heal(Engine *_engine, Player *_player, float _quantity);

void Player_UpdateDamageAndFireRate(Player *_player);

void Player_Update(Engine *_engine, Players *_players, float _dt, Lasers *_lasers);

void Player_Display(Engine *_engine, Players *_players);

void Player_DestroyAll(Players *_players);

#endif // !PLAYER__H