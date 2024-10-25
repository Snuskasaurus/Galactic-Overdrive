#include "Menu.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void Menu_TextEntered(MainData *_data, char _c);
void Menu_KeyPressed(MainData *_data, sfKeyCode _key);
void Menu_KeyReleased(MainData *_data, sfKeyCode _key);
void Menu_MouseButtonScrolled(MainData *_data, float _delta);
void Menu_MouseButtonPressed(MainData *_data, sfMouseButton _button);
void Menu_MouseButtonReleased(MainData *_data, sfMouseButton _button);
void Menu_MouseMoved(MainData *_data);
void Menu_JoystickButtonPressed(MainData *_data, joystickId _joystick, joystickButton _button);
void Menu_JoystickButtonReleased(MainData *_data, joystickId _joystick, joystickButton _button);
void Menu_JoystickMoved(MainData *_data, joystickId _joystick, sfJoystickAxis _axis, float _position);
void Menu_JoystickConnected(MainData *_data, joystickId _joystick);
void Menu_JoystickDisconnected(MainData *_data, joystickId _joystick);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Menu_HandleEvent(MainData *_data, sfEvent _event)
{
	switch (_event.type)
	{
	case sfEvtTextEntered:
		Menu_TextEntered(_data, (char)_event.text.unicode);
		break;
	case sfEvtKeyPressed:
		Menu_KeyPressed(_data, _event.key.code);
		break;
	case sfEvtKeyReleased:
		Menu_KeyReleased(_data, _event.key.code);
		break;
	case sfEvtMouseWheelScrolled:
		Menu_MouseButtonScrolled(_data, _event.mouseWheelScroll.delta);
		break;
	case sfEvtMouseButtonPressed:
		Menu_MouseButtonPressed(_data, _event.mouseButton.button);
		break;
	case sfEvtMouseButtonReleased:
		Menu_MouseButtonReleased(_data, _event.mouseButton.button);
		break;
	case sfEvtMouseMoved:
		Menu_MouseMoved(_data);
		break;
	case sfEvtJoystickButtonPressed:
		Menu_JoystickButtonPressed(_data, _event.joystickButton.joystickId, _event.joystickButton.button);
		break;
	case sfEvtJoystickButtonReleased:
		Menu_JoystickButtonReleased(_data, _event.joystickButton.joystickId, _event.joystickButton.button);
		break;
	case sfEvtJoystickMoved:
		Menu_JoystickMoved(_data, _event.joystickMove.joystickId, _event.joystickMove.axis, _event.joystickMove.position);
		break;
	case sfEvtJoystickConnected:
		Menu_JoystickConnected(_data, _event.joystickConnect.joystickId);
		break;
	case sfEvtJoystickDisconnected:
		Menu_JoystickDisconnected(_data, _event.joystickConnect.joystickId);
		break;
	default:
		break;
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Menu_TextEntered(MainData *_data, char _c)
{
	char tmp_s[2];
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];

	// Create the string
	tmp_s[0] = _c;
	tmp_s[1] = '\0';
}

void Menu_KeyPressed(MainData *_data, sfKeyCode _key)
{
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];
	if (state->subState == MENU_ACHIEVEMENTS)
	{
		if (sfKeyboard_isKeyPressed(sfKeyRight) || sfKeyboard_isKeyPressed(sfKeyD))
		{
			AchievementMenu_ArrowPageRight(_data);
		}
		if (sfKeyboard_isKeyPressed(sfKeyLeft) || sfKeyboard_isKeyPressed(sfKeyQ))
		{
			AchievementMenu_ArrowPageLeft(_data);
		}
		if (sfKeyboard_isKeyPressed(sfKeyEscape))
		{
			AchievementMenu_BackToMenu(_data);
		}
	}
}

void Menu_KeyReleased(MainData *_data, sfKeyCode _key)
{
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];
}

void Menu_MouseButtonScrolled(MainData *_data, float _delta)
{
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];
	// Get the mouse position in the screen
	sfVector2i mousePosition_i = sfMouse_getPositionRenderWindow(_data->engine.window);
	sfVector2f mousePosition_f = { (float)mousePosition_i.x , (float)mousePosition_i.y };
}

void Menu_MouseButtonPressed(MainData *_data, sfMouseButton _button)
{
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];
	// Get the mouse position in the screen
	sfVector2i mousePosition_i = sfMouse_getPositionRenderWindow(_data->engine.window);
	sfVector2f mousePosition_f = { (float)mousePosition_i.x , (float)mousePosition_i.y };

	Button_Pressing(&_data->engine, &state->buttons[state->subState]);
}

void Menu_MouseButtonReleased(MainData *_data, sfMouseButton _button)
{
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];
	// Get the mouse position in the screen
	sfVector2i mousePosition_i = sfMouse_getPositionRenderWindow(_data->engine.window);
	sfVector2f mousePosition_f = { (float)mousePosition_i.x , (float)mousePosition_i.y };

	Button_Releasing(&_data->engine, _data, &state->buttons[state->subState]);
}

void Menu_MouseMoved(MainData *_data)
{
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];
	// Get the mouse position in the screen
	sfVector2i mousePosition_i = sfMouse_getPositionRenderWindow(_data->engine.window);
	sfVector2f mousePosition_f = { (float)mousePosition_i.x , (float)mousePosition_i.y };

	if (state->buttons[state->subState].array[0]->manual != sfFalse)
	{
		Button_SetManual(&state->buttons[state->subState], sfFalse);
	}
}

void Menu_JoystickButtonPressed(MainData *_data, joystickId _joystick, joystickButton _button)
{
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];

	switch (_button)
	{
	case XBOX_A:
	{
		switch (state->subState)
		{
		case MENU_MAIN:
		{
			Button_Pressing(&_data->engine, &state->buttons[state->subState]);
			break;
		}
		case MENU_OPTION:
		{
			Button_Pressing(&_data->engine, &state->buttons[state->subState]);
			break;
		}
		case MENU_ACHIEVEMENTS:
		{
			Button_Pressing(&_data->engine, &state->buttons[state->subState]);
			break;
		}
		default:
			break;
		}
		break;
	}
	case XBOX_LB:
	{
		switch (state->subState)
		{
		case MENU_ACHIEVEMENTS:
		{
			AchievementMenu_ArrowPageLeft(_data);
			break;
		}
		default:
			break;
		}
		break;
	}
	case XBOX_RB:
	{
		switch (state->subState)
		{
		case MENU_ACHIEVEMENTS:
		{
			AchievementMenu_ArrowPageRight(_data);
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

void Menu_JoystickButtonReleased(MainData *_data, joystickId _joystick, joystickButton _button)
{
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];

	switch (_button)
	{
	case XBOX_A:
	{
		Button_Releasing(&_data->engine, _data, &state->buttons[state->subState]);
		if (state->subState == state->previousSubState)
		{
			state->buttons[state->subState].array[state->currentButton]->currentState = BUTTON_HOVERED;
		}
		break;
	}
	case XBOX_B:
	{
		state->subState = MENU_MAIN;
		break;
	}
	default:
		break;
	}
}

void Menu_JoystickMoved(MainData *_data, joystickId _joystick, sfJoystickAxis _axis, float _position)
{
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];

	switch (_axis)
	{
	case sfJoystickPovY:
	{
		Menu_ChangeCurrentButton(&_data->engine, state, _position, state->buttons[state->subState].number);
		break;
	}
	case sfJoystickPovX:
		if (state->subState == MENU_ACHIEVEMENTS)
		{
			printf("PovX = %f\n", _position);
		}
		break;
	default:
		break;
	}
}

void Menu_JoystickConnected(MainData *_data, joystickId _joystick)
{
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];
}

void Menu_JoystickDisconnected(MainData *_data, joystickId _joystick)
{
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];
}

sfBool Menu_ChangeCurrentButton(Engine *_engine, MenuData *_state, float _position, int _maxNbButtons)
{
	if (_position >= JOYSTICK_MINIMUM_SENSITIVITY)
	{
		_state->currentButton--;
		if (_state->currentButton < 0)
		{
			_state->currentButton = _maxNbButtons - 1;
		}
		Button_Select(_engine, &_state->buttons[_state->subState], _state->buttons[_state->subState].array[_state->currentButton]);
		return sfTrue;
	}
	else if (_position <= -JOYSTICK_MINIMUM_SENSITIVITY)
	{
		_state->currentButton++;
		_state->currentButton = _state->currentButton % _maxNbButtons;
		Button_Select(_engine, &_state->buttons[_state->subState], _state->buttons[_state->subState].array[_state->currentButton]);
		return sfTrue;
	}
	return sfFalse;
}