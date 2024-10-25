#include "Overworld.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void Overworld_TextEntered(MainData *_data, char _c);
void Overworld_KeyPressed(MainData *_data, sfKeyCode _key);
void Overworld_KeyReleased(MainData *_data, sfKeyCode _key);
void Overworld_MouseButtonScrolled(MainData *_data, float _delta);
void Overworld_MouseButtonPressed(MainData *_data, sfMouseButton _button);
void Overworld_MouseButtonReleased(MainData *_data, sfMouseButton _button);
void Overworld_MouseMoved(MainData *_data);
void Overworld_JoystickButtonPressed(MainData *_data, joystickId _joystick, joystickButton _button);
void Overworld_JoystickButtonReleased(MainData *_data, joystickId _joystick, joystickButton _button);
void Overworld_JoystickMoved(MainData *_data, joystickId _joystick, sfJoystickAxis _axis, float _position);
void Overworld_JoystickConnected(MainData *_data, joystickId _joystick);
void Overworld_JoystickDisconnected(MainData *_data, joystickId _joystick);

// -- MAIN FUNCTION -------------------------------------------------------- //

void OverworldToMenu(MainData* _data)
{
	_data->currentState = MENU;
}

void Overworld_HandleEvent(MainData *_data, sfEvent _event)
{
	switch (_event.type)
	{
	case sfEvtTextEntered:
		Overworld_TextEntered(_data, (char)_event.text.unicode);
		break;
	case sfEvtKeyPressed:
		Overworld_KeyPressed(_data, _event.key.code);
		break;
	case sfEvtKeyReleased:
		Overworld_KeyReleased(_data, _event.key.code);
		break;
	case sfEvtMouseWheelScrolled:
		Overworld_MouseButtonScrolled(_data, _event.mouseWheelScroll.delta);
		break;
	case sfEvtMouseButtonPressed:
		Overworld_MouseButtonPressed(_data, _event.mouseButton.button);
		break;
	case sfEvtMouseButtonReleased:
		Overworld_MouseButtonReleased(_data, _event.mouseButton.button);
		break;
	case sfEvtMouseMoved:
		Overworld_MouseMoved(_data);
		break;
	case sfEvtJoystickButtonPressed:
		Overworld_JoystickButtonPressed(_data, _event.joystickButton.joystickId, _event.joystickButton.button);
		break;
	case sfEvtJoystickButtonReleased:
		Overworld_JoystickButtonReleased(_data, _event.joystickButton.joystickId, _event.joystickButton.button);
		break;
	case sfEvtJoystickMoved:
		Overworld_JoystickMoved(_data, _event.joystickMove.joystickId, _event.joystickMove.axis, _event.joystickMove.position);
		break;
	case sfEvtJoystickConnected:
		Overworld_JoystickConnected(_data, _event.joystickConnect.joystickId);
		break;
	case sfEvtJoystickDisconnected:
		Overworld_JoystickDisconnected(_data, _event.joystickConnect.joystickId);
		break;
	default:
		break;
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Overworld_TextEntered(MainData *_data, char _c)
{
	OverworldData *state = _data->stateData[OVERWORLD];
}

void Overworld_KeyPressed(MainData *_data, sfKeyCode _key)
{
	OverworldData *state = _data->stateData[OVERWORLD];
}

void LeaveSelection(MainData* _data)
{
	OverworldData *state = _data->stateData[OVERWORLD];
	for (int i = 0; i < NB_LEVELS; i++)
	{
		_data->engine.currentDifficulty = SetDifficulty(_data->engine.blueprints.difficultyBlueprint, state->nbPlayers, state->choosenDifficulty, 1);
		state->state = state->Levels[i].selected ? LOBBY : state->state;
	}
}

void Overworld_KeyReleased(MainData *_data, sfKeyCode _key)
{
	OverworldData *state = _data->stateData[OVERWORLD];
	int connected = sfFalse;
	int index = 0;
	for (int i = 0; i < state->nbPlayers; i++)
	{
		connected = state->lobby[i].joystick == 4 ? sfTrue : connected;
		index = state->lobby[i].joystick == 4 ? i : index;
	}
	if (_key == sfKeyEscape)
	{
		Fade_CreateTransition(&_data->transitionFade, OverworldToMenu);
	}
	switch (state->state)
	{
	case SELECTION:
		switch (_key)
		{
		case sfKeySpace:
			if (state->nbPlayers && connected)
			{
				Fade_CreateTransition(&_data->transitionFade, LeaveSelection);
			}
			if (!connected && state->nbPlayers < state->toConnect)
			{
				state->lobby[state->nbPlayers].joystick = 4;
				state->nbPlayers++;
			}
			break;
		case sfKeyRShift:
			if (connected)
			{
				for (int i = state->nbPlayers - 1; i >= 0; i--)
				{
					if (state->lobby[i].joystick == 4)
					{
						state->lobby[i].joystick = -1;
						for (int j = i; j < state->nbPlayers - 1; j++)
						{
							struct Lobby tempLobby = state->lobby[j];
							state->lobby[j] = state->lobby[j + 1];
							state->lobby[j + 1] = tempLobby;
						}
					}
				}
				state->nbPlayers--;
			}
			else
			{
				Fade_CreateTransition(&_data->transitionFade, OverworldToMenu);
			}
			break;
		case sfKeyTab:
			if (state->nbPlayers && connected)
			{
				state->choosenDifficulty = (state->choosenDifficulty + 1) % 3;
			}
			break;
		case sfKeyF1:
			for (int i = 0; i < NB_LEVELS; i++)
			{
				state->Levels[i].unlocked = sfTrue;
			}
			break;
		default:
			break;
		}
		break;
	case LOBBY:
		if (connected)
		{
			switch (_key)
			{
			case sfKeySpace:
				if (state->lobby[index].Window != READY)
				{
					state->lobby[index].Window++;
				}
				break;
			case sfKeyRShift:
				if (state->lobby[index].Window != LEAVE)
				{
					state->lobby[index].Window--;
				}
				break;
			case sfKeyQ:
				switch (state->lobby[index].Window)
				{
				case SPACESHIP:
					state->lobby[index].choiceSpaceship--;
					if (state->lobby[index].choiceSpaceship < 0)
					{
						state->lobby[index].choiceSpaceship = _data->engine.blueprints.players.number - 1;
					}
					break;
				case COLOR:
					state->lobby[index].choiceColor--;
					if (state->lobby[index].choiceColor < 0)
					{
						state->lobby[index].choiceColor = _data->nbColors - 1;
					}
					break;
				case SPECIAL:
					state->lobby[index].choiceSpecial--;
					if (state->lobby[index].choiceSpecial < 0)
					{
						state->lobby[index].choiceSpecial = _data->engine.skills.nbr - 1;
					}
					break;
				default:
					break;
				}
				state->lobby[index].select_frame = 1;
				state->lobby[index].currentTime = 0;
				break;
			case sfKeyD:
				switch (state->lobby[index].Window)
				{
				case SPACESHIP:
					state->lobby[index].choiceSpaceship++;
					if (state->lobby[index].choiceSpaceship >= _data->engine.blueprints.players.number)
					{
						state->lobby[index].choiceSpaceship = 0;
					}
					break;
				case COLOR:
					state->lobby[index].choiceColor++;
					if (state->lobby[index].choiceColor >= _data->nbColors)
					{
						state->lobby[index].choiceColor = 0;
					}
					break;
				case SPECIAL:
					state->lobby[index].choiceSpecial++;
					if (state->lobby[index].choiceSpecial >= _data->engine.skills.nbr)
					{
						state->lobby[index].choiceSpecial = 0;
					}
					break;
				default:
					break;
				}
				state->lobby[index].select_frame = 2;
				state->lobby[index].currentTime = 0;
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}

void Overworld_MouseButtonScrolled(MainData *_data, float _delta)
{
	OverworldData *state = _data->stateData[OVERWORLD];
}

void Overworld_MouseButtonPressed(MainData *_data, sfMouseButton _button)
{
	OverworldData *state = _data->stateData[OVERWORLD];
}

void Overworld_MouseButtonReleased(MainData *_data, sfMouseButton _button)
{
	OverworldData *state = _data->stateData[OVERWORLD];
}

void Overworld_MouseMoved(MainData *_data)
{
	OverworldData *state = _data->stateData[OVERWORLD];
}

void Overworld_JoystickButtonPressed(MainData *_data, joystickId _joystick, joystickButton _button)
{
	OverworldData *state = _data->stateData[OVERWORLD];
}

void Overworld_JoystickButtonReleased(MainData *_data, joystickId _joystick, joystickButton _button)
{
	OverworldData *state = _data->stateData[OVERWORLD];
	int connected = sfFalse;
	int index = 0;
	for (int i = 0; i < state->nbPlayers; i++)
	{
		connected = state->lobby[i].joystick == _joystick ? sfTrue : connected;
		index = state->lobby[i].joystick == _joystick ? i : index;
	}
	switch (state->state)
	{
	case SELECTION:
		switch (_button)
		{
		case 0:
			if (state->nbPlayers && connected)
			{
				/*for (int i = 0; i < NB_LEVELS; i++)
				{
					_data->engine.currentDifficulty = SetDifficulty(_data->engine.blueprints.difficultyBlueprint, state->nbPlayers, state->choosenDifficulty, 1);
					state->state = state->Levels[i].selected ? LOBBY : state->state;
				}*/
				Fade_CreateTransition(&_data->transitionFade, LeaveSelection);
			}
			if (!connected && state->nbPlayers < state->toConnect)
			{
				state->lobby[state->nbPlayers].joystick = _joystick;
				state->nbPlayers++;
			}
			break;
		case 1:
			if (state->nbPlayers && connected)
			{
				for (int i = state->nbPlayers - 1; i >= 0; i--)
				{
					if (state->lobby[i].joystick == _joystick)
					{
						state->lobby[i].joystick = -1;
						for (int j = i; j < state->nbPlayers - 1; j++)
						{
							struct Lobby tempLobby = state->lobby[j];
							state->lobby[j] = state->lobby[j + 1];
							state->lobby[j + 1] = tempLobby;
						}
					}
				}
				state->nbPlayers--;
			}
			else
			{
				Fade_CreateTransition(&_data->transitionFade, OverworldToMenu);
			}
			break;
		case 3:
			if (state->nbPlayers && connected)
			{
				state->choosenDifficulty = (state->choosenDifficulty + 1) % 3;
			}
			break;
		default:
			break;
		}
		break;
	case LOBBY:
		if (connected)
		{
			switch (_button)
			{
			case 0:
				if (state->lobby[index].Window != READY)
				{
					state->lobby[index].Window++;
				}
				break;
			case 1:
				if (state->lobby[index].Window != LEAVE)
				{
					state->lobby[index].Window--;
				}
				break;
			default:
				break;
			}
		}
	default:
		break;
	}
}

void Overworld_JoystickMoved(MainData *_data, joystickId _joystick, sfJoystickAxis _axis, float _position)
{
	OverworldData *state = _data->stateData[OVERWORLD];
	int connected = sfFalse;
	int index = 0;
	for (int i = 0; i < state->nbPlayers; i++)
	{
		connected = state->lobby[i].joystick == _joystick ? sfTrue : connected;
		index = state->lobby[i].joystick == _joystick ? i : index;
	}
	//printf("%d\n", index);
	if (connected && state->state == LOBBY)
	{
		if (_axis == sfJoystickPovX)
		{
			if (_position < 0)
			{
				switch (state->lobby[index].Window)
				{
				case SPACESHIP:
					state->lobby[index].choiceSpaceship--;
					if (state->lobby[index].choiceSpaceship < 0)
					{
						state->lobby[index].choiceSpaceship = _data->engine.blueprints.players.number - 1;
					}
					break;
				case COLOR:
					state->lobby[index].choiceColor--;
					if (state->lobby[index].choiceColor < 0)
					{
						state->lobby[index].choiceColor = _data->nbColors - 1;
					}
					break;
				case SPECIAL:
					state->lobby[index].choiceSpecial--;
					if (state->lobby[index].choiceSpecial < 0)
					{
						state->lobby[index].choiceSpecial = _data->engine.skills.nbr - 1;
					}
					break;
				default:
					break;
				}
				state->lobby[index].select_frame = 1;
				state->lobby[index].currentTime = 0;
			}
			else if (_position > 0)
			{
				switch (state->lobby[index].Window)
				{
				case SPACESHIP:
					state->lobby[index].choiceSpaceship++;
					if (state->lobby[index].choiceSpaceship >= _data->engine.blueprints.players.number)
					{
						state->lobby[index].choiceSpaceship = 0;
					}
					break;
				case COLOR:
					state->lobby[index].choiceColor++;
					if (state->lobby[index].choiceColor >= _data->nbColors)
					{
						state->lobby[index].choiceColor = 0;
					}
					break;
				case SPECIAL:
					state->lobby[index].choiceSpecial++;
					if (state->lobby[index].choiceSpecial >= _data->engine.skills.nbr)
					{
						state->lobby[index].choiceSpecial = 0;
					}
					break;
				default:
					break;
				}
				state->lobby[index].select_frame = 2;
				state->lobby[index].currentTime = 0;
			}
		}
	}
}

void Overworld_JoystickConnected(MainData *_data, joystickId _joystick)
{
	OverworldData *state = _data->stateData[OVERWORLD];
}

void Overworld_JoystickDisconnected(MainData *_data, joystickId _joystick)
{
	OverworldData *state = _data->stateData[OVERWORLD];
	if (state->state == SELECTION)
	{
		int connected = sfFalse;
		for (int i = 0; i < state->nbPlayers; i++)
		{
			connected = state->lobby[i].joystick == _joystick ? sfTrue : connected;
		}
		if (connected)
		{
			for (int i = state->nbPlayers - 1; i >= 0; i--)
			{
				if (state->lobby[i].joystick == _joystick)
				{
					state->lobby[i].joystick = -1;
					for (int j = i; j < state->nbPlayers - 1; j++)
					{
						struct Lobby tempLobby = state->lobby[j];
						state->lobby[j] = state->lobby[j + 1];
						state->lobby[j + 1] = tempLobby;
					}
				}
			}
			state->nbPlayers--;
		}
	}
}

