#include "Shop.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void Shop_TextEntered(MainData *_data, char _c);
void Shop_KeyPressed(MainData *_data, sfKeyCode _key);
void Shop_KeyReleased(MainData *_data, sfKeyCode _key);
void Shop_MouseButtonScrolled(MainData *_data, float _delta);
void Shop_MouseButtonPressed(MainData *_data, sfMouseButton _button);
void Shop_MouseButtonReleased(MainData *_data, sfMouseButton _button);
void Shop_JoystickButtonPressed(MainData *_data, joystickId _joystick, joystickButton _button);
void Shop_JoystickButtonReleased(MainData *_data, joystickId _joystick, joystickButton _button);
void Shop_JoystickMoved(MainData *_data, joystickId _joystick, sfJoystickAxis _axis, float _position);
void Shop_JoystickConnected(MainData *_data, joystickId _joystick);
void Shop_JoystickDisconnected(MainData *_data, joystickId _joystick);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Shop_HandleEvent(MainData *_data, sfEvent _event)
{
	switch (_event.type)
	{
	case sfEvtTextEntered:
		Shop_TextEntered(_data, (char)_event.text.unicode);
		break;
	case sfEvtKeyPressed:
		Shop_KeyPressed(_data, _event.key.code);
		break;
	case sfEvtKeyReleased:
		Shop_KeyReleased(_data, _event.key.code);
		break;
	case sfEvtMouseWheelScrolled:
		Shop_MouseButtonScrolled(_data, _event.mouseWheelScroll.delta);
		break;
	case sfEvtMouseButtonPressed:
		Shop_MouseButtonPressed(_data, _event.mouseButton.button);
		break;
	case sfEvtMouseButtonReleased:
		Shop_MouseButtonReleased(_data, _event.mouseButton.button);
		break;
	case sfEvtJoystickButtonPressed:
		Shop_JoystickButtonPressed(_data, _event.joystickButton.joystickId, _event.joystickButton.button);
		break;
	case sfEvtJoystickButtonReleased:
		Shop_JoystickButtonReleased(_data, _event.joystickButton.joystickId, _event.joystickButton.button);
		break;
	case sfEvtJoystickMoved:
		Shop_JoystickMoved(_data, _event.joystickMove.joystickId, _event.joystickMove.axis, _event.joystickMove.position);
		break;
	case sfEvtJoystickConnected:
		Shop_JoystickConnected(_data, _event.joystickConnect.joystickId);
		break;
	case sfEvtJoystickDisconnected:
		Shop_JoystickDisconnected(_data, _event.joystickConnect.joystickId);
		break;
	default:
		break;
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Shop_TextEntered(MainData *_data, char _c)
{
	char tmp_s[2];
	// Create a shortcut to the state data
	GameData *state = _data->stateData[SHOP];

	// Create the string
	tmp_s[0] = _c;
	tmp_s[1] = '\0';

	///printf("Text entered %s\n", tmp_s);
}

void Shop_KeyPressed(MainData *_data, sfKeyCode _key)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[SHOP];
	///printf("Key pressed\n");
	int index = -1;
	for (int i = 0; i < state->players.number; i++)
	{
		index = 4 == state->players.array[i]->joystick ? i : index;
	}
	if (index >= 0)
	{
		Weapon* TempWeapon = state->players.array[index]->inventory;
		Weapon* TempBuy = NULL;
		Weapon* TempSell = NULL;
		Weapon* TempPrevious = NULL;
		Weapon* TempSwitch = NULL;
		int nbOwnedWeapons = 0;
		int weaponIndex = 0;
		
		switch (_key)
		{
		case sfKeyF6:
		{
			for (int i = 0; i < state->players.number; i++)
			{
				state->players.array[i]->money += 30000;
			}
			break;
		}
		case sfKeySpace:
			switch (state->currentShopState[index])
			{
			case SHOP_BUY:
				for (int i = 0; i < _data->engine.blueprints.weapons.number; i++)
				{
					if (_data->engine.blueprints.weapons.array[i]->isVisibleInShop)
					{
						if (nbOwnedWeapons == state->shopCursorPos[index])
						{
							weaponIndex = i;
						}
						nbOwnedWeapons++;
					}
				}
				if (state->isMakingTransaction[index])
				{
					if (state->players.array[index]->money >= _data->engine.blueprints.weapons.array[weaponIndex]->price)
					{
						TempBuy = (Weapon*)malloc(sizeof(Weapon));
						(*TempBuy) = GetWeapon(_data->engine.blueprints.weapons.array[weaponIndex]->name, &_data->engine.blueprints.weapons, WEAPON_OWNER_PLAYER, state->players.array[index]);
						while (TempWeapon && TempWeapon->Next)
						{
							TempWeapon = TempWeapon->Next;
						}
						if (TempWeapon)
						{
							TempWeapon->Next = TempBuy;
						}
						else
						{
							state->players.array[index]->inventory = TempBuy;
						}
						state->players.array[index]->money -= _data->engine.blueprints.weapons.array[weaponIndex]->price;
						state->players.array[index]->inventorySize++;
						state->isMakingTransaction[index] = sfFalse;
					}
					else
					{
						//
					}
				}
				else
				{
					int canEquip = sfFalse;
					for (int i = 0; i < state->players.array[index]->nbSlotsMax; i++)
					{
						canEquip = _data->engine.blueprints.weapons.array[weaponIndex]->type == state->players.array[index]->weapons[i].weapon.type ? sfTrue : canEquip;
					}
					if (canEquip)
					{
						state->isMakingTransaction[index] = sfTrue;
					}
				}
				break;
			case SHOP_SELL:
				if (state->isMakingTransaction[index])
				{
					TempSell = state->players.array[index]->inventory;
					while (TempSell && nbOwnedWeapons != state->shopCursorPos[index])
					{
						TempPrevious = TempSell;
						TempSell = TempSell->Next;
						nbOwnedWeapons++;
					}
					if (TempPrevious)
					{
						TempPrevious->Next = TempSell->Next;
					}
					else
					{
						state->players.array[index]->inventory = TempSell->Next;
					}
					state->players.array[index]->money += TempSell->price;
					free(TempSell);
					state->players.array[index]->inventorySize--;
					if (state->shopCursorPos[index] == state->players.array[index]->inventorySize)
					{
						state->shopCursorPos[index]--;
					}
					state->isMakingTransaction[index] = sfFalse;
				}
				else
				{
					if (state->players.array[index]->inventorySize)
					{
						state->isMakingTransaction[index] = sfTrue;
					}
				}
				break;
			case SHOP_EQUIP:
				if (state->shopIsEquiping[index])
				{
					while (TempWeapon && nbOwnedWeapons != state->shopEquipCursorPos[index])
					{
						if (TempWeapon->type == state->players.array[index]->weapons[state->shopCursorPos[index]].weapon.type)
						{
							nbOwnedWeapons++;
						}
						TempPrevious = TempWeapon;
						TempWeapon = TempWeapon->Next;
					}
					while (TempWeapon->type != state->players.array[index]->weapons[state->shopCursorPos[index]].weapon.type)
					{
						TempWeapon = TempWeapon->Next;
					}
					TempSwitch = (Weapon*)malloc(sizeof(Weapon));
					(*TempSwitch) = GetWeapon(state->players.array[index]->weapons[state->shopCursorPos[index]].weapon.name, &_data->engine.blueprints.weapons, WEAPON_OWNER_PLAYER, state->players.array[index]);
					state->players.array[index]->weapons[state->shopCursorPos[index]].weapon = GetWeapon(TempWeapon->name, &_data->engine.blueprints.weapons, WEAPON_OWNER_PLAYER, state->players.array[index]);
					state->players.array[index]->weaponSlots[state->shopCursorPos[index]].weapon = GetWeapon(TempWeapon->name, &_data->engine.blueprints.weapons, WEAPON_OWNER_PLAYER, state->players.array[index]);
					TempSwitch->Next = TempWeapon->Next;
					if (TempPrevious)
					{
						TempPrevious->Next = TempSwitch;
					}
					else
					{
						state->players.array[index]->inventory = TempSwitch;
					}
					free(TempWeapon);
					state->shopIsEquiping[index] = sfFalse;
				}
				else
				{
					while (TempWeapon)
					{
						if (TempWeapon->type == state->players.array[index]->weapons[state->shopCursorPos[index]].weapon.type)
						{
							nbOwnedWeapons++;
						}
						TempWeapon = TempWeapon->Next;
					}
					if (nbOwnedWeapons)
					{
						state->shopIsEquiping[index] = sfTrue;
						state->shopEquipCursorPos[index] = 0;
						state->shopEquipCursorOffset[index] = 0;
					}
				}
				break;
			default:
				break;
			}
			break;
		case sfKeyRShift:
			if ((state->currentShopState[index] == SHOP_BUY || state->currentShopState[index] == SHOP_SELL) && state->isMakingTransaction)
			{
				state->isMakingTransaction[index] = sfFalse;
			}
			if (state->currentShopState[index] == SHOP_EQUIP && state->shopIsEquiping[index])
			{
				state->shopIsEquiping[index] = sfFalse;
			}
			break;
		case sfKeyEnter:
			if (!state->shopIsEquiping[index] && !state->isMakingTransaction[index])
			{
				state->currentShopState[index] = SHOP_READY;
			}
			break;
		case sfKeyA:
			if (!state->shopIsEquiping[index] && !state->isMakingTransaction[index])
			{
				state->currentShopState[index]--;
				if (state->currentShopState[index] < SHOP_BUY)
				{
					state->currentShopState[index] = SHOP_EQUIP;
				}
				state->shopCursorPos[index] = 0;
				state->shopCursorOffset[index] = 0;
			}
			break;
		case sfKeyE:
			if (!state->shopIsEquiping[index] && !state->isMakingTransaction[index])
			{
				state->currentShopState[index]++;
				if (state->currentShopState[index] > SHOP_EQUIP)
				{
					state->currentShopState[index] = SHOP_BUY;
				}
				state->shopCursorPos[index] = 0;
				state->shopCursorOffset[index] = 0;
			}
			break;
		default:
			break;
		case sfKeyZ:
			if ((state->currentShopState[index] == SHOP_BUY || state->currentShopState[index] == SHOP_SELL) && !state->isMakingTransaction[index])
			{
				state->shopCursorPos[index] -= 4;
			}
			if (state->shopIsEquiping[index])
			{
				state->shopEquipCursorPos[index] -= 4;
			}
			break;
		case sfKeyQ:
			if (state->shopIsEquiping[index])
			{
				state->shopEquipCursorPos[index]--;
			}
			else if (!state->isMakingTransaction[index])
			{
				state->shopCursorPos[index]--;
			}
			break;
		case sfKeyS:
			if ((state->currentShopState[index] == SHOP_BUY || state->currentShopState[index] == SHOP_SELL) && !state->isMakingTransaction[index])
			{
				state->shopCursorPos[index] += 4;
			}
			if (state->shopIsEquiping[index])
			{
				state->shopEquipCursorPos[index] += 4;
			}
			break;
		case sfKeyD:
			if (state->shopIsEquiping[index])
			{
				state->shopEquipCursorPos[index]++;
			}
			else if (!state->isMakingTransaction[index])
			{
				state->shopCursorPos[index]++;
			}
			break;
		}
		switch (state->currentShopState[index])
		{
		case SHOP_BUY:
			for (int i = 0; i < _data->engine.blueprints.weapons.number; i++)
			{
				if (_data->engine.blueprints.weapons.array[i]->isVisibleInShop)
				{
					nbOwnedWeapons++;
				}
			}
			if (state->shopCursorPos[index] >= nbOwnedWeapons)
			{
				state->shopCursorPos[index] = nbOwnedWeapons - 1;
			}
			break;
		case SHOP_SELL:
			if (state->shopCursorPos[index] >= state->players.array[index]->inventorySize)
			{
				state->shopCursorPos[index] = state->players.array[index]->inventorySize - 1;
			}
			break;
		case SHOP_EQUIP:
			if (state->shopIsEquiping[index])
			{
				Weapon* TempWeapon = state->players.array[index]->inventory;
				while (TempWeapon)
				{
					if (TempWeapon->type == state->players.array[index]->weapons[state->shopCursorPos[index]].weapon.type)
					{
						nbOwnedWeapons++;
					}
					TempWeapon = TempWeapon->Next;
				}
				if (state->shopEquipCursorPos[index] >= nbOwnedWeapons)
				{
					state->shopEquipCursorPos[index] = nbOwnedWeapons - 1;
				}
			}
			else
			{
				if (state->shopCursorPos[index] >= state->players.array[index]->nbSlotsMax)
				{
					state->shopCursorPos[index] = state->players.array[index]->nbSlotsMax - 1;
				}
			}
			break;
		default:
			break;
		}
		if (state->shopCursorPos[index] < 0)
		{
			state->shopCursorPos[index] = 0;
		}
		if (state->shopEquipCursorPos[index] < 0)
		{
			state->shopEquipCursorPos[index] = 0;
		}
		if (state->shopCursorPos[index] / 4 - state->shopCursorOffset[index] < 0)
		{
			state->shopCursorOffset[index]--;
		}
		else if (state->shopCursorPos[index] / 4 - state->shopCursorOffset[index] >= 4)
		{
			state->shopCursorOffset[index]++;
		}
		if (state->shopEquipCursorPos[index] / 4 - state->shopEquipCursorOffset[index] < 0)
		{
			state->shopEquipCursorOffset[index]--;
		}
		else if (state->shopEquipCursorPos[index] / 4 - state->shopEquipCursorOffset[index] >= 4)
		{
			state->shopEquipCursorOffset[index]++;
		}
	}
}

void Shop_KeyReleased(MainData *_data, sfKeyCode _key)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[SHOP];

	///printf("Key released\n");
	if (_key == sfKeyEscape)
	{
		_data->currentState = MENU;
	}
}

void Shop_MouseButtonScrolled(MainData *_data, float _delta)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[SHOP];

	///printf("Mouse button scrolled for %i delta\n", _delta);
}

void Shop_MouseButtonPressed(MainData *_data, sfMouseButton _button)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[SHOP];
	// Create a shortcut to the key used in keybinding
	///Keybinding *key = _data->keybinding;

	///printf("Mouse button pressed\n");
}

void Shop_MouseButtonReleased(MainData *_data, sfMouseButton _button)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[SHOP];
	// Create a shortcut to the key used in keybinding
	///Keybinding *key = _data->keybinding;

	///printf("Mouse button released\n");
}

void Shop_JoystickButtonPressed(MainData *_data, joystickId _joystick, joystickButton _button)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[SHOP];
	///printf("Joystick %d button %d pressed\n", _joystick, _button);
	Player_JoystickButtonPressed(_joystick, _button, state, SHOP, &_data->engine);
	int index = -1;
	for (int i = 0; i < state->players.number; i++)
	{
		index = _joystick == state->players.array[i]->joystick ? i : index;
	}
	if (index >= 0)
	{
		Weapon* TempWeapon = state->players.array[index]->inventory;
		Weapon* TempBuy = NULL;
		Weapon* TempSell = NULL;
		Weapon* TempPrevious = NULL;
		Weapon* TempSwitch = NULL;
		int nbOwnedWeapons = 0;
		int weaponIndex = 0;
		switch (_button)
		{
		case 0:
			switch (state->currentShopState[index])
			{
			case SHOP_BUY:
				for (int i = 0; i < _data->engine.blueprints.weapons.number; i++)
				{
					if (_data->engine.blueprints.weapons.array[i]->isVisibleInShop)
					{
						if (nbOwnedWeapons == state->shopCursorPos[index])
						{
							weaponIndex = i;
						}
						nbOwnedWeapons++;
					}
				}
				if (state->isMakingTransaction[index])
				{
					if (state->players.array[index]->money >= _data->engine.blueprints.weapons.array[weaponIndex]->price)
					{
						TempBuy = (Weapon*)malloc(sizeof(Weapon));
						(*TempBuy) = GetWeapon(_data->engine.blueprints.weapons.array[weaponIndex]->name, &_data->engine.blueprints.weapons, WEAPON_OWNER_PLAYER, state->players.array[index]);
						while (TempWeapon && TempWeapon->Next)
						{
							TempWeapon = TempWeapon->Next;
						}
						if (TempWeapon)
						{
							TempWeapon->Next = TempBuy;
						}
						else
						{
							state->players.array[index]->inventory = TempBuy;
						}
						state->players.array[index]->money -= _data->engine.blueprints.weapons.array[weaponIndex]->price;
						state->players.array[index]->inventorySize++;
						state->isMakingTransaction[index] = sfFalse;
					}
					else
					{
						//
					}
				}
				else
				{
					int canEquip = sfFalse;
					for (int i = 0; i < state->players.array[index]->nbSlotsMax; i++)
					{
						canEquip = _data->engine.blueprints.weapons.array[weaponIndex]->type == state->players.array[index]->weapons[i].weapon.type ? sfTrue : canEquip;
					}
					if (canEquip)
					{
						state->isMakingTransaction[index] = sfTrue;
					}
				}
				break;
			case SHOP_SELL:
				if (state->isMakingTransaction[index])
				{
					TempSell = state->players.array[index]->inventory;
					while (TempSell && nbOwnedWeapons != state->shopCursorPos[index])
					{
						TempPrevious = TempSell;
						TempSell = TempSell->Next;
						nbOwnedWeapons++;
					}
					if (TempPrevious)
					{
						TempPrevious->Next = TempSell->Next;
					}
					else
					{
						state->players.array[index]->inventory = TempSell->Next;
					}
					state->players.array[index]->money += TempSell->price;
					free(TempSell);
					state->players.array[index]->inventorySize--;
					if (state->shopCursorPos[index] == state->players.array[index]->inventorySize)
					{
						state->shopCursorPos[index]--;
					}
					state->isMakingTransaction[index] = sfFalse;
				}
				else
				{
					if (state->players.array[index]->inventorySize)
					{
						state->isMakingTransaction[index] = sfTrue;
					}
				}
				break;
			case SHOP_EQUIP:
				if (state->shopIsEquiping[index])
				{
					while (TempWeapon && nbOwnedWeapons != state->shopEquipCursorPos[index])
					{
						if (TempWeapon->type == state->players.array[index]->weapons[state->shopCursorPos[index]].weapon.type)
						{
							nbOwnedWeapons++;
						}
						TempPrevious = TempWeapon;
						TempWeapon = TempWeapon->Next;
					}
					while (TempWeapon->type != state->players.array[index]->weapons[state->shopCursorPos[index]].weapon.type)
					{
						TempWeapon = TempWeapon->Next;
					}
					TempSwitch = (Weapon*)malloc(sizeof(Weapon));
					(*TempSwitch) = GetWeapon(state->players.array[index]->weapons[state->shopCursorPos[index]].weapon.name, &_data->engine.blueprints.weapons, WEAPON_OWNER_PLAYER, state->players.array[index]);
					state->players.array[index]->weapons[state->shopCursorPos[index]].weapon = GetWeapon(TempWeapon->name, &_data->engine.blueprints.weapons, WEAPON_OWNER_PLAYER, state->players.array[index]);
					state->players.array[index]->weaponSlots[state->shopCursorPos[index]].weapon = GetWeapon(TempWeapon->name, &_data->engine.blueprints.weapons, WEAPON_OWNER_PLAYER, state->players.array[index]);
					TempSwitch->Next = TempWeapon->Next;
					if (TempPrevious)
					{
						TempPrevious->Next = TempSwitch;
					}
					else
					{
						state->players.array[index]->inventory = TempSwitch;
					}
					free(TempWeapon);
					state->shopIsEquiping[index] = sfFalse;
				}
				else
				{
					while (TempWeapon)
					{
						if (TempWeapon->type == state->players.array[index]->weapons[state->shopCursorPos[index]].weapon.type)
						{
							nbOwnedWeapons++;
						}
						TempWeapon = TempWeapon->Next;
					}
					if (nbOwnedWeapons)
					{
						state->shopIsEquiping[index] = sfTrue;
						state->shopEquipCursorPos[index] = 0;
						state->shopEquipCursorOffset[index] = 0;
					}
				}
				break;
			default:
				break;
			}
			break;
		case 1:
			if ((state->currentShopState[index] == SHOP_BUY || state->currentShopState[index] == SHOP_SELL) && state->isMakingTransaction)
			{
				state->isMakingTransaction[index] = sfFalse;
			}
			if (state->currentShopState[index] == SHOP_EQUIP && state->shopIsEquiping[index])
			{
				state->shopIsEquiping[index] = sfFalse;
			}
			break;
		case 2:
			switch (state->currentShopState[index])
			{
			default:
				if (!state->shopIsEquiping[index] && !state->isMakingTransaction[index])
				{
					state->currentShopState[index]++;
					state->currentShopState[index] %= NB_SHOP_STATES - 1;
					state->shopCursorPos[index] = 0;
					state->shopCursorOffset[index] = 0;
				}
				break;
			case SHOP_READY:
				state->currentShopState[index] = SHOP_EQUIP;
				state->shopCursorPos[index] = 0;
				state->shopCursorOffset[index] = 0;
				break;
			}
			break;
		case 3:
			if (!state->shopIsEquiping[index] && !state->isMakingTransaction[index])
			{
				state->currentShopState[index] = SHOP_READY;
			}
			break;
		case 4:
			if (!state->shopIsEquiping[index] && !state->isMakingTransaction[index])
			{
				state->currentShopState[index]--;
				if (state->currentShopState[index] < SHOP_BUY)
				{
					state->currentShopState[index] = SHOP_EQUIP;
				}
				state->shopCursorPos[index] = 0;
				state->shopCursorOffset[index] = 0;
			}
			break;
		case 5:
			if (!state->shopIsEquiping[index] && !state->isMakingTransaction[index])
			{
				state->currentShopState[index]++;
				if (state->currentShopState[index] > SHOP_EQUIP)
				{
					state->currentShopState[index] = SHOP_BUY;
				}
				state->shopCursorPos[index] = 0;
				state->shopCursorOffset[index] = 0;
			}
			break;
		default:
			break;
		}
	}
}

void Shop_JoystickButtonReleased(MainData *_data, joystickId _joystick, joystickButton _button)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];
	///printf("Joystick %d button %d released\n", _joystick, _button);
}

void Shop_JoystickMoved(MainData *_data, joystickId _joystick, sfJoystickAxis _axis, float _position)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[SHOP];
	///printf("Joystick %d moved axis %d for %f\n", _joystick, _axis, _position);
	int nbOwnedWeapon = 0;
	int index = -1;
	for (int i = 0; i < state->players.number; i++)
	{
		index = _joystick == state->players.array[i]->joystick ? i : index;
	}
	switch (_axis)
	{
	case sfJoystickPovX:
		if (state->shopIsEquiping[index])
		{
			if (_position < 0)
			{
				state->shopEquipCursorPos[index]--;
			}
			else if (_position > 0)
			{
				state->shopEquipCursorPos[index]++;
			}
		}
		else if (!state->isMakingTransaction[index])
		{
			if (_position < 0)
			{
				state->shopCursorPos[index]--;
			}
			else if (_position > 0)
			{
				state->shopCursorPos[index]++;
			}
		}
		break;
	case sfJoystickPovY:
		if ((state->currentShopState[index] == SHOP_BUY || state->currentShopState[index] == SHOP_SELL) && !state->isMakingTransaction[index])
		{
			if (_position < 0)
			{
				state->shopCursorPos[index] += 4;
			}
			else if (_position > 0)
			{
				state->shopCursorPos[index] -= 4;
			}
		}
		if (state->shopIsEquiping[index])
		{
			if (_position < 0)
			{
				state->shopEquipCursorPos[index] += 4;
			}
			else if (_position > 0)
			{
				state->shopEquipCursorPos[index] -= 4;
			}
		}
		break;
	default:
		break;
	}
	switch (state->currentShopState[index])
	{
	case SHOP_BUY:
		for (int i = 0; i < _data->engine.blueprints.weapons.number; i++)
		{
			if (_data->engine.blueprints.weapons.array[i]->isVisibleInShop)
			{
				nbOwnedWeapon++;
			}
		}
		if (state->shopCursorPos[index] >= nbOwnedWeapon)
		{
			state->shopCursorPos[index] = nbOwnedWeapon - 1;
		}
		break;
	case SHOP_SELL:
		if (state->shopCursorPos[index] >= state->players.array[index]->inventorySize)
		{
			state->shopCursorPos[index] = state->players.array[index]->inventorySize - 1;
		}
		break;
	case SHOP_EQUIP:
		if (state->shopIsEquiping[index])
		{
			Weapon* TempWeapon = state->players.array[index]->inventory;
			while (TempWeapon)
			{
				if (TempWeapon->type == state->players.array[index]->weapons[state->shopCursorPos[index]].weapon.type)
				{
					nbOwnedWeapon++;
				}
				TempWeapon = TempWeapon->Next;
			}
			if (state->shopEquipCursorPos[index] >= nbOwnedWeapon)
			{
				state->shopEquipCursorPos[index] = nbOwnedWeapon - 1;
			}
		}
		else
		{
			if (state->shopCursorPos[index] >= state->players.array[index]->nbSlotsMax)
			{
				state->shopCursorPos[index] = state->players.array[index]->nbSlotsMax - 1;
			}
		}
		break;
	default:
		break;
	}
	if (state->shopCursorPos[index] < 0)
	{
		state->shopCursorPos[index] = 0;
	}
	if (state->shopEquipCursorPos[index] < 0)
	{
		state->shopEquipCursorPos[index] = 0;
	}
	if (state->shopCursorPos[index] / 4 - state->shopCursorOffset[index] < 0)
	{
		state->shopCursorOffset[index]--;
	}
	else if (state->shopCursorPos[index] / 4 - state->shopCursorOffset[index] >= 4)
	{
		state->shopCursorOffset[index]++;
	}
	if (state->shopEquipCursorPos[index] / 4 - state->shopEquipCursorOffset[index] < 0)
	{
		state->shopEquipCursorOffset[index]--;
	}
	else if (state->shopEquipCursorPos[index] / 4 - state->shopEquipCursorOffset[index] >= 4)
	{
		state->shopEquipCursorOffset[index]++;
	}
}

void Shop_JoystickConnected(MainData *_data, joystickId _joystick)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[SHOP];
	///printf("Joystick %d connected\n", _joystick);
}

void Shop_JoystickDisconnected(MainData *_data, joystickId _joystick)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[SHOP];
	///printf("Joystick %d disconnected\n", _joystick);
}