#include "Credit.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void Credit_TextEntered(MainData *_data, char _c);
void Credit_KeyPressed(MainData *_data, sfKeyCode _key);
void Credit_KeyReleased(MainData *_data, sfKeyCode _key);
void Credit_MouseButtonScrolled(MainData *_data, float _delta);
void Credit_MouseButtonPressed(MainData *_data, sfMouseButton _button);
void Credit_MouseButtonReleased(MainData *_data, sfMouseButton _button);

// -- MAIN FUNCTION -------------------------------------------------------- //

void QuitCredits(MainData* _data)
{
	_data->currentState = MENU;
}

void Credit_HandleEvent(MainData *_data, sfEvent _event)
{
	switch (_event.type)
	{
	case sfEvtTextEntered:
		Credit_TextEntered(_data, (char)_event.text.unicode);
		break;
	case sfEvtKeyPressed:
		Credit_KeyPressed(_data, _event.key.code);
		Fade_CreateTransition(&_data->transitionFade, QuitCredits);
		break;
	case sfEvtKeyReleased:
		Credit_KeyReleased(_data, _event.key.code);
		break;
	case sfEvtMouseWheelScrolled:
		Credit_MouseButtonScrolled(_data, _event.mouseWheelScroll.delta);
		break;
	case sfEvtMouseButtonPressed:
		Credit_MouseButtonPressed(_data, _event.mouseButton.button);
		Fade_CreateTransition(&_data->transitionFade, QuitCredits);
		break;
	case sfEvtMouseButtonReleased:
		Credit_MouseButtonReleased(_data, _event.mouseButton.button);
		break;
	default:
		break;
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Credit_TextEntered(MainData *_data, char _c)
{
	char tmp_s[2];
	// Create a shortcut to the state data
	CreditData *state = _data->stateData[CREDIT];

	// Create the string
	tmp_s[0] = _c;
	tmp_s[1] = '\0';

	printf("Text entered %s\n", tmp_s);
}

void Credit_KeyPressed(MainData *_data, sfKeyCode _key)
{
	// Create a shortcut to the state data
	CreditData *state = _data->stateData[CREDIT];

	printf("Key pressed\n");
}

void Credit_KeyReleased(MainData *_data, sfKeyCode _key)
{
	// Create a shortcut to the state data
	CreditData *state = _data->stateData[CREDIT];

	printf("Key released\n");
}

void Credit_MouseButtonScrolled(MainData *_data, float _delta)
{
	// Create a shortcut to the state data
	CreditData *state = _data->stateData[CREDIT];

	printf("Mouse button scrolled for %f delta\n", _delta);
}

void Credit_MouseButtonPressed(MainData *_data, sfMouseButton _button)
{
	// Create a shortcut to the state data
	CreditData *state = _data->stateData[CREDIT];
	// Create a shortcut to the key used in keybinding
	///Keybinding *key = _data->keybinding;

	printf("Mouse button pressed\n");
}

void Credit_MouseButtonReleased(MainData *_data, sfMouseButton _button)
{ 
	// Create a shortcut to the state data
	CreditData *state = _data->stateData[CREDIT];
	// Create a shortcut to the key used in keybinding
	///Keybinding *key = _data->keybinding;

	printf("Mouse button released\n");
}