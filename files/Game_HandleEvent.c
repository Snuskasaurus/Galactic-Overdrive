#include "Game.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void Game_TextEntered(MainData *_data, char _c);
void Game_KeyPressed(MainData *_data, sfKeyCode _key);
void Game_KeyReleased(MainData *_data, sfKeyCode _key);
void Game_MouseButtonScrolled(MainData *_data, float _delta);
void Game_MouseButtonPressed(MainData *_data, sfMouseButton _button);
void Game_MouseButtonReleased(MainData *_data, sfMouseButton _button);
void Game_JoystickButtonPressed(MainData *_data, joystickId _joystick, joystickButton _button);
void Game_JoystickButtonReleased(MainData *_data, joystickId _joystick, joystickButton _button);
void Game_JoystickMoved(MainData *_data, joystickId _joystick, sfJoystickAxis _axis, float _position);
void Game_JoystickConnected(MainData *_data, joystickId _joystick);
void Game_JoystickDisconnected(MainData *_data, joystickId _joystick);
void Game_GameToEditor(MainData *_data);
void Game_GameToMenu(MainData *_data);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Game_HandleEvent(MainData *_data, sfEvent _event)
{
	switch (_event.type)
	{
	case sfEvtTextEntered:
		Game_TextEntered(_data, (char)_event.text.unicode);
		break;
	case sfEvtKeyPressed:
		Game_KeyPressed(_data, _event.key.code);
		break;
	case sfEvtKeyReleased:
		Game_KeyReleased(_data, _event.key.code);
		break;
	case sfEvtMouseWheelScrolled:
		Game_MouseButtonScrolled(_data, _event.mouseWheelScroll.delta);
		break;
	case sfEvtMouseButtonPressed:
		Game_MouseButtonPressed(_data, _event.mouseButton.button);
		break;
	case sfEvtMouseButtonReleased:
		Game_MouseButtonReleased(_data, _event.mouseButton.button);
		break;
	case sfEvtJoystickButtonPressed:
		Game_JoystickButtonPressed(_data, _event.joystickButton.joystickId, _event.joystickButton.button);
		break;
	case sfEvtJoystickButtonReleased:
		Game_JoystickButtonReleased(_data, _event.joystickButton.joystickId, _event.joystickButton.button);
		break;
	case sfEvtJoystickMoved:
		Game_JoystickMoved(_data, _event.joystickMove.joystickId, _event.joystickMove.axis, _event.joystickMove.position);
		break;
	case sfEvtJoystickConnected:
		Game_JoystickConnected(_data, _event.joystickConnect.joystickId);
		break;
	case sfEvtJoystickDisconnected:
		Game_JoystickDisconnected(_data, _event.joystickConnect.joystickId);
		break;
	default:
		break;
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Game_TextEntered(MainData *_data, char _c)
{
	char tmp_s[2];
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];

	// Create the string
	tmp_s[0] = _c;
	tmp_s[1] = '\0';
}

void Game_KeyPressed(MainData *_data, sfKeyCode _key)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];

	switch (_key)
	{
	case sfKeyA:
		Skill_EventButtonPressed(KEYBOARD_JOYSTICK_ID, SKILL_BUTTON, state, &_data->engine);
		break;
	case sfKeySpace:
	{
		if (state->isDefeated)
		{
			_data->engine.world.currentLevel = 0;
			_data->currentState = MENU;
		}
		break;
	}
	case sfKeyF1:
	{
		if (state->isIndestructible)
		{
			state->isIndestructible = FALSE;
		}
		else
		{
			state->isIndestructible = TRUE;
		}
		break;
	}
	case sfKeyF2:
	{
		if (state->level.map.time < state->level.bossTime - 0.5f)
		{
			state->level.map.time = state->level.bossTime - 0.5f;
		}
		break;
	}
	case sfKeyF3:
	{
		if (state->boss.isAlive)
		{
			state->boss.isAlive = FALSE;
		}
		break;
	}
	case sfKeyF4:
	{
		if (state->hasInterfaceDisplayed)
		{
			state->hasInterfaceDisplayed = FALSE;
		}
		else
		{
			state->hasInterfaceDisplayed = TRUE;
		}
		break;
	}
	default:
		break;
	}
}

void Game_KeyReleased(MainData *_data, sfKeyCode _key)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];

	///printf("Key released\n");
	if (_key == sfKeyEscape)
	{
		if (_data->isLaunchedFromEditor)
		{
			_data->engine.world.currentLevel = 0;
			Fade_CreateTransition(&_data->transitionFade, &Game_GameToEditor);
		}
		else
		{
			Fade_CreateTransition(&_data->transitionFade, &Game_GameToMenu);
		}
	}
}

void Game_MouseButtonScrolled(MainData *_data, float _delta)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];
}

void Game_MouseButtonPressed(MainData *_data, sfMouseButton _button)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];
	// Create a shortcut to the key used in keybinding
	///Keybinding *key = _data->keybinding;

	///printf("Mouse button pressed\n");
}

void Game_MouseButtonReleased(MainData *_data, sfMouseButton _button)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];
	// Create a shortcut to the key used in keybinding
	///Keybinding *key = _data->keybinding;

	///printf("Mouse button released\n");
}

void Game_JoystickButtonPressed(MainData *_data, joystickId _joystick, joystickButton _button)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];
	///printf("Joystick %d button %d pressed\n", _joystick, _button);
	Player_JoystickButtonPressed(_joystick, _button, state, GAME, &_data->engine);
}

void Game_JoystickButtonReleased(MainData *_data, joystickId _joystick, joystickButton _button)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];
	///printf("Joystick %d button %d released\n", _joystick, _button);
	Player_JoystickButtonReleased(_joystick, _button, state);
}

void Game_JoystickMoved(MainData *_data, joystickId _joystick, sfJoystickAxis _axis, float _position)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];
	///printf("Joystick %d moved axis %d for %f\n", _joystick, _axis, _position);

	Player_JoystickMoved(_data, _joystick, _axis, _position);
}

void Game_JoystickConnected(MainData *_data, joystickId _joystick)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];
	///printf("Joystick %d connected\n", _joystick);
}

void Game_JoystickDisconnected(MainData *_data, joystickId _joystick)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];
	///printf("Joystick %d disconnected\n", _joystick);
}

void Game_GameToEditor(MainData *_data)
{
	Sound_StopAll(_data->engine.sound);
	_data->currentState = EDITOR;
}

void Game_GameToMenu(MainData *_data)
{
	_data->currentState = MENU;
}