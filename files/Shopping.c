#include "Shopping.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- SHARED FUNCTIONS ----------------------------------------------------- //

void Shopping_Update(GameData *_state, Engine *_engine, float _dt)
{
	for (int i = 0; i < _state->players.number; i++)
	{
		Player *player = _state->players.array[i];
	}
}

void Shop_DisplayIndividualScore(GameData* _state, Engine* _engine)
{
	sfVector2i tempResolution = _engine->configuration->resolution;
	sfVector2f tempScale = _engine->configuration->scale;
	Weapon* TempWeapon = NULL;
	int nbOwnedWeapon = 0;
	for (int i = 0; i < _state->players.number; i++)
	{
		int money[7] = { 0, 0, 0, 0, 0, 0, _state->players.array[i]->money };
		for (int j = 6; j > 0; j--)
		{
			while (money[j] >= 10)
			{
				money[j] -= 10;
				money[j - 1]++;
			}
		}
		for (int j = 0; j < 7; j++)
		{
			Sprite_Display(_engine->canvas, _state->numbers[money[j]], (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8 - 34 * 3 + j * 10 * 3), (float)(15.0f * tempScale.y) }, (sfVector2f) { 3, 3 }, 0, NULL);
		}
		/*switch (_state->currentShopState[i])
		{
		case SHOP_BUY:
			_state->parametersText.color = sfRed;
			sprintf(money, "- %u", _engine->blueprints.weapons.array[_state->shopCursorPos[i]]->price);
			Text_Display(_engine->canvas, &_state->parametersText, money, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), 75.0f * tempScale.y }, (sfVector2f) { 0.5f, 0.5f });
			break;
		case SHOP_SELL:
			if (_state->players.array[i]->inventorySize)
			{
				TempWeapon = _state->players.array[i]->inventory;
				while (TempWeapon && nbOwnedWeapon != _state->shopCursorPos[i])
				{
					TempWeapon = TempWeapon->Next;
					nbOwnedWeapon++;
				}
				_state->parametersText.color = sfGreen;
				sprintf(money, "+ %u", TempWeapon->price);
				Text_Display(_engine->canvas, &_state->parametersText, money, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), 75.0f * tempScale.y }, (sfVector2f) { 0.5f, 0.5f });
			}
			break;
		default:
			break;
		}
		_state->parametersText.color = sfWhite;*/
	}
}

void Shopping_Display(GameData *_state, Engine *_engine)
{
	sfVector2i tempResolution = _engine->configuration->resolution;
	sfVector2f tempScale = _engine->configuration->scale;
	Sprite_Display(_engine->canvas, _state->shopSprite, (sfVector2f) { 0, 0 }, (sfVector2f) { 3, 3 }, 0, NULL);
	_state->parametersText.color = sfYellow;
	for (int i = 0; i < _state->players.number; i++)
	{
		int nbOwnedWeapons = 0;
		Weapon* TempWeapon = _state->players.array[i]->inventory;
		sfVector2f tempCursorPos;
		char weaponName[BUFFER_SMALL];
		Sprite_Display(_engine->canvas, _state->shopPlayerID[i], (sfVector2f) { (float)((i * 8 + 1) * tempResolution.x / 32 + 9 + i * 3), (float)(tempResolution.y - tempResolution.x / 4 - 30) }, (sfVector2f) { 3, 3 }, 0, NULL);
		if (_state->currentShopState[i] != SHOP_READY)
		{
			Sprite_Display(_engine->canvas, _state->shopState[_state->currentShopState[i]], (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8 + i * 3), (float)(tempResolution.y - tempResolution.x / 4 - 48) }, (sfVector2f) { 3, 3 }, 0, NULL);
		}
		switch (_state->currentShopState[i])
		{
		case SHOP_BUY:
			if (_state->isMakingTransaction[i])
			{
				char stats[BUFFER_SMALL] = { 0 };
				char* type[WEAPON_NB_TYPES] = { "cinetic", "energetic", "explosive", "healing", "boost" };
				sprintf(stats, "type : %s", type[_state->testWeapon[i]->type]);
				Text_Display(_engine->canvas, &_state->parametersText, stats, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), (float)(tempResolution.y - tempResolution.x / 4 + tempResolution.x / 24) }, (sfVector2f) { 0.5f, 0.5f });
				sprintf(stats, "dmg : %d", (unsigned int)_state->testWeapon[i]->damage);
				Text_Display(_engine->canvas, &_state->parametersText, stats, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), (float)(tempResolution.y - tempResolution.x / 4 + 2 * tempResolution.x / 24) }, (sfVector2f) { 0.5f, 0.5f });
				sprintf(stats, "speed : %0.1f", _state->testWeapon[i]->rate.maxTime);
				Text_Display(_engine->canvas, &_state->parametersText, stats, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), (float)(tempResolution.y - tempResolution.x / 4 + 3 * tempResolution.x / 24) }, (sfVector2f) { 0.5f, 0.5f });
				sprintf(stats, "prec. : %d", (unsigned int)_state->testWeapon[i]->precision);
				Text_Display(_engine->canvas, &_state->parametersText, stats, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), (float)(tempResolution.y - tempResolution.x / 4 + 4 * tempResolution.x / 24) }, (sfVector2f) { 0.5f, 0.5f });
				sprintf(stats, "price : %u", _state->testWeapon[i]->price);
				Text_Display(_engine->canvas, &_state->parametersText, stats, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), (float)(tempResolution.y - tempResolution.x / 4 + 5 * tempResolution.x / 24) }, (sfVector2f) { 0.5f, 0.5f });

				char tempName[BUFFER_SMALL] = { 0 };
				char *returned = NULL;
				strcpy(tempName, _state->testWeapon[i]->name);
				returned = strchr(tempName, '_');

				while (returned != NULL)
				{
					int index = returned - tempName;
					tempName[index] = ' ';
					returned = strchr(tempName, '_');
				}

				Text_Display(_engine->canvas, &_state->parametersText, tempName, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), 45.0f * tempScale.y }, (sfVector2f) { 0.5f, 0.5f });
			}
			else
			{
				for (int j = 0; j < _engine->blueprints.weapons.number; j++)
				{
					if (_engine->blueprints.weapons.array[j]->isVisibleInShop)
					{
						if (nbOwnedWeapons / 4 >= 0 + _state->shopCursorOffset[i] && nbOwnedWeapons / 4 < 4 + _state->shopCursorOffset[i])
						{
							sfVector2f tempPos = { (float)(i * tempResolution.x / 4 + 24 + (nbOwnedWeapons - 4 * (int)(nbOwnedWeapons / 4)) * 112), (float)(tempResolution.y - tempResolution.x / 4 + 24 + (int)(nbOwnedWeapons / 4 - _state->shopCursorOffset[i]) * 112) };
							Sprite_Display(_engine->canvas, _state->frames[_engine->blueprints.weapons.array[j]->upgradeTier], tempPos, (sfVector2f) { 3, 3 }, 0, NULL);
							Sprite_Display(_engine->canvas, _engine->blueprints.weapons.array[j]->spriteW, (sfVector2f) { tempPos.x + 48, tempPos.y + 48 }, (sfVector2f) { 2, 2 }, 0, NULL);
						}
						if (nbOwnedWeapons == _state->shopCursorPos[i])
						{
							strcpy(weaponName, _engine->blueprints.weapons.array[j]->name);

							char tempName[BUFFER_SMALL] = { 0 };
							char *returned = NULL;
							strcpy(tempName, weaponName);
							returned = strchr(tempName, '_');

							while (returned != NULL)
							{
								int index = returned - tempName;
								tempName[index] = ' ';
								returned = strchr(tempName, '_');
							}

							strcpy(weaponName, tempName);
						}
						nbOwnedWeapons++;
					}
				}
				tempCursorPos.x = (float)(i * tempResolution.x / 4 + 24 + (_state->shopCursorPos[i] - 4 * (int)(_state->shopCursorPos[i] / 4)) * 112);
				tempCursorPos.y = (float)(tempResolution.y - tempResolution.x / 4 + 24 + (int)(_state->shopCursorPos[i] / 4 - _state->shopCursorOffset[i]) * 112);
				Sprite_Display(_engine->canvas, _state->shopCursor, tempCursorPos, (sfVector2f) { 3, 3 }, 0, NULL);
				Text_Display(_engine->canvas, &_state->parametersText, weaponName, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), 45.0f * tempScale.y }, (sfVector2f) { 0.5f, 0.5f });
			}
			break;
		case SHOP_SELL:
			if (_state->isMakingTransaction[i])
			{
				char stats[BUFFER_SMALL] = { 0 };
				char* type[WEAPON_NB_TYPES] = { "cinetic", "energetic", "explosive", "healing", "boost" };
				sprintf(stats, "type : %s", type[_state->testWeapon[i]->type]);
				Text_Display(_engine->canvas, &_state->parametersText, stats, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), (float)(tempResolution.y - tempResolution.x / 4 + tempResolution.x / 24) }, (sfVector2f) { 0.5f, 0.5f });
				sprintf(stats, "dmg : %d", (unsigned int)_state->testWeapon[i]->damage);
				Text_Display(_engine->canvas, &_state->parametersText, stats, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), (float)(tempResolution.y - tempResolution.x / 4 + 2 * tempResolution.x / 24) }, (sfVector2f) { 0.5f, 0.5f });
				sprintf(stats, "speed : %0.1f", _state->testWeapon[i]->rate.maxTime);
				Text_Display(_engine->canvas, &_state->parametersText, stats, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), (float)(tempResolution.y - tempResolution.x / 4 + 3 * tempResolution.x / 24) }, (sfVector2f) { 0.5f, 0.5f });
				sprintf(stats, "prec. : %d", (unsigned int)_state->testWeapon[i]->precision);
				Text_Display(_engine->canvas, &_state->parametersText, stats, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), (float)(tempResolution.y - tempResolution.x / 4 + 4 * tempResolution.x / 24) }, (sfVector2f) { 0.5f, 0.5f });
				sprintf(stats, "price : %u", _state->testWeapon[i]->price);
				Text_Display(_engine->canvas, &_state->parametersText, stats, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), (float)(tempResolution.y - tempResolution.x / 4 + 5 * tempResolution.x / 24) }, (sfVector2f) { 0.5f, 0.5f });

				char tempName[BUFFER_SMALL] = { 0 };
				char *returned = NULL;
				strcpy(tempName, _state->testWeapon[i]->name);
				returned = strchr(tempName, '_');

				while (returned != NULL)
				{
					int index = returned - tempName;
					tempName[index] = ' ';
					returned = strchr(tempName, '_');
				}

				Text_Display(_engine->canvas, &_state->parametersText, tempName, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), 45.0f * tempScale.y }, (sfVector2f) { 0.5f, 0.5f });
			}
			else
			{
				while (TempWeapon)
				{
					if (nbOwnedWeapons / 4 >= 0 + _state->shopCursorOffset[i] && nbOwnedWeapons / 4 < 4 + _state->shopCursorOffset[i])
					{
						sfVector2f tempPos = { (float)(i * tempResolution.x / 4 + 24 + (nbOwnedWeapons - 4 * (int)(nbOwnedWeapons / 4)) * 112), (float)(tempResolution.y - tempResolution.x / 4 + 24 + (int)(nbOwnedWeapons / 4 - _state->shopCursorOffset[i]) * 112) };
						Sprite_Display(_engine->canvas, _state->frames[TempWeapon->upgradeTier], tempPos, (sfVector2f) { 3, 3 }, 0, NULL);
						Sprite_Display(_engine->canvas, TempWeapon->spriteW, (sfVector2f) { tempPos.x + 48, tempPos.y + 48 }, (sfVector2f) { 2, 2 }, 0, NULL);
						if (nbOwnedWeapons == _state->shopCursorPos[i])
						{
							strcpy(weaponName, TempWeapon->name);

							char tempName[BUFFER_SMALL] = { 0 };
							char *returned = NULL;
							strcpy(tempName, weaponName);
							returned = strchr(tempName, '_');

							while (returned != NULL)
							{
								int index = returned - tempName;
								tempName[index] = ' ';
								returned = strchr(tempName, '_');
							}

							strcpy(weaponName, tempName);
						}
					}
					TempWeapon = TempWeapon->Next;
					nbOwnedWeapons++;
				}
				if (_state->players.array[i]->inventorySize)
				{
					tempCursorPos.x = (float)(i * tempResolution.x / 4 + 24 + (_state->shopCursorPos[i] - 4 * (int)(_state->shopCursorPos[i] / 4)) * 112);
					tempCursorPos.y = (float)(tempResolution.y - tempResolution.x / 4 + 24 + (int)(_state->shopCursorPos[i] / 4 - _state->shopCursorOffset[i]) * 112);
					Sprite_Display(_engine->canvas, _state->shopCursor, tempCursorPos, (sfVector2f) { 3, 3 }, 0, NULL);
					Text_Display(_engine->canvas, &_state->parametersText, weaponName, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), 45.0f * tempScale.y }, (sfVector2f) { 0.5f, 0.5f });
				}
			}
			break;
		case SHOP_EQUIP:
			if (_state->shopIsEquiping[i])
			{
				while (TempWeapon)
				{
					if (TempWeapon->type == _state->players.array[i]->weapons[_state->shopCursorPos[i]].weapon.type)
					{
						if (nbOwnedWeapons / 4 >= 0 + _state->shopEquipCursorOffset[i] && nbOwnedWeapons / 4 < 4 + _state->shopEquipCursorOffset[i])
						{
							sfVector2f tempPos = { (float)(i * tempResolution.x / 4 + 24 + (nbOwnedWeapons - 4 * (int)(nbOwnedWeapons / 4)) * 112), (float)(tempResolution.y - tempResolution.x / 4 + 24 + (int)(nbOwnedWeapons / 4 - _state->shopEquipCursorOffset[i]) * 112) };
							Sprite_Display(_engine->canvas, _state->frames[TempWeapon->upgradeTier], tempPos, (sfVector2f) { 3, 3 }, 0, NULL);
							Sprite_Display(_engine->canvas, TempWeapon->spriteW, (sfVector2f) { tempPos.x + 48, tempPos.y + 48 }, (sfVector2f) { 2, 2 }, 0, NULL);
							if (nbOwnedWeapons == _state->shopEquipCursorPos[i])
							{
								strcpy(weaponName, TempWeapon->name);

								char tempName[BUFFER_SMALL] = { 0 };
								char *returned = NULL;
								strcpy(tempName, weaponName);
								returned = strchr(tempName, '_');

								while (returned != NULL)
								{
									int index = returned - tempName;
									tempName[index] = ' ';
									returned = strchr(tempName, '_');
								}

								strcpy(weaponName, tempName);
							}
						}
						nbOwnedWeapons++;
					}
					TempWeapon = TempWeapon->Next;
				}
				tempCursorPos.x = (float)(i * tempResolution.x / 4 + 24 + (_state->shopEquipCursorPos[i] - 4 * (int)(_state->shopEquipCursorPos[i] / 4)) * 112);
				tempCursorPos.y = (float)(tempResolution.y - tempResolution.x / 4 + 24 + (int)(_state->shopEquipCursorPos[i] / 4 - _state->shopEquipCursorOffset[i]) * 112);
				Sprite_Display(_engine->canvas, _state->shopCursor, tempCursorPos, (sfVector2f) { 3, 3 }, 0, NULL);
				Text_Display(_engine->canvas, &_state->parametersText, weaponName, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), 45.0f * tempScale.y }, (sfVector2f) { 0.5f, 0.5f });
			}
			else
			{
				for (int j = 0; j < _state->players.array[i]->nbSlotsMax; j++)
				{
					sfVector2f tempPos = { (float)(i * tempResolution.x / 4 + j * tempResolution.x / (4 * _state->players.array[i]->nbSlotsMax) + 24), (float)(tempResolution.y - tempResolution.x / 8 - 48) };
					Sprite_Display(_engine->canvas, _state->frames[_state->players.array[i]->weapons[j].weapon.upgradeTier], tempPos, (sfVector2f) { 3, 3 }, 0, NULL);
					Sprite_Display(_engine->canvas, _state->players.array[i]->weapons[j].weapon.spriteW, (sfVector2f) { tempPos.x + 48, tempPos.y + 48 }, (sfVector2f) { 2, 2 }, 0, NULL);
					if (j == _state->shopCursorPos[i])
					{
						strcpy(weaponName, _state->players.array[i]->weapons[j].weapon.name);

						char tempName[BUFFER_SMALL] = { 0 };
						char *returned = NULL;
						strcpy(tempName, weaponName);
						returned = strchr(tempName, '_');

						while (returned != NULL)
						{
							int index = returned - tempName;
							tempName[index] = ' ';
							returned = strchr(tempName, '_');
						}

						strcpy(weaponName, tempName);
					}
				}
				tempCursorPos.x = (float)(i * tempResolution.x / 4 + _state->shopCursorPos[i] * tempResolution.x / (4 * _state->players.array[i]->nbSlotsMax) + 24);
				tempCursorPos.y = (float)(tempResolution.y - tempResolution.x / 8 - 48);
				Sprite_Display(_engine->canvas, _state->shopCursor, tempCursorPos, (sfVector2f) { 3, 3 }, 0, NULL);
				Text_Display(_engine->canvas, &_state->parametersText, weaponName, (sfVector2f) { (float)((i * 2 + 1) * tempResolution.x / 8), 45.0f * tempScale.y }, (sfVector2f) { 0.5f, 0.5f });
			}
			break;
		default:
			break;
		}
	}
	_state->parametersText.color = sfWhite;
	Shop_DisplayIndividualScore(_state, _engine);
}

Weapon Shopping_BuyWeapon(BP_Weapons *_weaponsBP, const char *_weaponName, Player *_player, GameData *_state, Weapon *_weaponToBeUpgraded, Engine *_engine)
{
	Weapon newWeapon = GetWeapon(_weaponName, _weaponsBP, WEAPON_OWNER_PLAYER, _player);
	int priceAfterDif = (int)(newWeapon.price * _engine->currentDifficulty.objectCostMultplier);

	if ((_state->collectedBills - priceAfterDif) >= 0)
	{
		_state->collectedBills -= priceAfterDif;
		return newWeapon;
	}
	else
	{
		printf("Not enough money...\n");
		// Gives back current weapon if not enough money...
		return (*_weaponToBeUpgraded);
	}
}

// Returns true if succesful purchase
sfBool Shopping_BuySkillCap(Player *_player, GameData *_state, int _price)
{
	if ((_state->collectedBills - _price) >= 0)
	{
		_player->skillInfo.skillStorageCap++;
		_state->collectedBills -= _price;
		return sfTrue;
	}
	printf("Not enough money...\n");
	return sfFalse;
}

// Returns true if succesful purchase
sfBool Shopping_BuySkillCharge(Player *_player, GameData *_state, int _price)
{
	if ((_player->skillInfo.nbSkillInStorage < _player->skillInfo.skillStorageCap) 
		&& ((_state->collectedBills - _price) >= 0))
	{
		_player->skillInfo.nbSkillInStorage++;
		_state->collectedBills -= _price;
		return sfTrue;
	}
	printf("Not enough money or skill storage cap...\n");
	return sfFalse;
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //