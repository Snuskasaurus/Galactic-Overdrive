#include "Shop.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void Shop_GetTestedWeapon(MainData* _data);
void Shop_CloseCondition(MainData*);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Shop_Update(MainData *_data, float _dt)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[SHOP];
	sfVector2i tempResolution = _data->engine.configuration->resolution;
	sfVector2f tempPos = { 0, 0 };
	float tempVolume = _data->engine.sound->volumes[0];

	Shopping_Update(state, &_data->engine, _dt);
	///Player_HandleShopMovementInput(&state->players);
	///HandleShootInput(&_data->engine, &state->players, &state->projectiles[WEAPON_OWNER_PLAYER], state->lasers);
	Shop_GetTestedWeapon(_data);
	_data->engine.sound->volumes[0] = 0;
	Player_ShopTestWeapon(&_data->engine, &state->players, &state->projectiles[WEAPON_OWNER_PLAYER], state->lasers, state->testWeapon);
	_data->engine.sound->volumes[0] = tempVolume;
	Player_Update(&_data->engine, &state->players, _dt, state->lasers);
	for (int i = 0; i < state->players.number; i++)
	{
		if (state->players.array[i]->entity.position.y >= tempResolution.y - tempResolution.x / 4.f)
		{
			state->players.array[i]->entity.position.y = tempResolution.y - tempResolution.x / 4.f;
		}
	}
	///Enemy_Update(&_data->engine, state->enemies, _dt, &state->projectiles[WEAPON_OWNER_ENEMY], &state->explosions, state->lasers, &state->moneyBills);
	for (int projectileType = 0; projectileType < WEAPON_OWNER_SIZE; projectileType++)
	{
		Projectile_Update(&_data->engine, _dt, &state->projectiles[projectileType], state, _data->currentState);
	}
	Laser_Update(state->lasers, &_data->engine);
	Game_HandleProjectilesCollisions(_data);
	///Game_HandleEnemiesCollisions(_data, &tempPos);
	Game_HandleLasersCollisions(_data, _dt, &tempPos);
	for (int projectileType = 0; projectileType < WEAPON_OWNER_SIZE; projectileType++)
	{
		List_Clear(&state->projectiles[projectileType]);
	}

	Shop_CloseCondition(_data);
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Shop_GetTestedWeapon(MainData* _data)
{
	GameData* state = _data->stateData[SHOP];
	for (int i = 0; i < state->players.number; i++)
	{
		Weapon* testedWeapon = state->testWeapon[i];
		Weapon* TempWeapon = state->players.array[i]->inventory;
		int nbOwnedWeapons = 0;
		int weaponIndex = 0;
		char wantToTest[BUFFER_SMALL] = { 0 };
		switch (state->currentShopState[i])
		{
		case SHOP_BUY:
			for (int j = 0; j < _data->engine.blueprints.weapons.number; j++)
			{
				if (_data->engine.blueprints.weapons.array[j]->isVisibleInShop)
				{
					if (nbOwnedWeapons == state->shopCursorPos[i])
					{
						weaponIndex = j;
					}
					nbOwnedWeapons++;
				}
			}
			strcpy(wantToTest, _data->engine.blueprints.weapons.array[weaponIndex]->name);
			break;
		case SHOP_SELL:
			if (state->players.array[i]->inventorySize)
			{
				while (TempWeapon && nbOwnedWeapons != state->shopCursorPos[i])
				{
					nbOwnedWeapons++;
					TempWeapon = TempWeapon->Next;
				}
				strcpy(wantToTest, TempWeapon->name);
			}
			break;
		case SHOP_EQUIP:
			if (state->shopIsEquiping[i])
			{
				if (state->players.array[i]->inventorySize)
				{
					while (TempWeapon && nbOwnedWeapons != state->shopEquipCursorPos[i])
					{
						if (TempWeapon->type == state->players.array[i]->weapons[state->shopCursorPos[i]].weapon.type)
						{
							nbOwnedWeapons++;
						}
						TempWeapon = TempWeapon->Next;
					}
					while (TempWeapon->type != state->players.array[i]->weapons[state->shopCursorPos[i]].weapon.type)
					{
						TempWeapon = TempWeapon->Next;
					}
					strcpy(wantToTest, TempWeapon->name);
				}
			}
			else
			{
				strcpy(wantToTest, state->players.array[i]->weapons[state->shopCursorPos[i]].weapon.name);
			}
			break;
		default:
			if (testedWeapon)
			{
				strcpy(wantToTest, testedWeapon->name);
			}
			break;
		}
		if (testedWeapon && _stricmp(testedWeapon->name, wantToTest))
		{
			if (testedWeapon->behaviour == WEAPON_STRAIGHTED_LASER_ORIENTED)
			{
				Laser_StopShooting(state->lasers, state->players.array[i], testedWeapon);
			}
			(*testedWeapon) = GetWeapon(wantToTest, &_data->engine.blueprints.weapons, WEAPON_OWNER_PLAYER, state->players.array[i]);
		}
	}
}

void ShopToGame(MainData* _data)
{
	_data->currentState = GAME;
}

void Shop_CloseCondition(MainData* _data)
{
	GameData *state = _data->stateData[SHOP];
	int isReady = sfTrue;
	for (int i = 0; i < state->players.number; i++)
	{
		isReady = state->currentShopState[i] != SHOP_READY ? sfFalse : isReady;
	}
	if (isReady)
	{
		Fade_CreateTransition(&_data->transitionFade, ShopToGame);
	}
}