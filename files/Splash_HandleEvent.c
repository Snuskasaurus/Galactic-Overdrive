#include "Splash.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void Splash_TextEntered(MainData *_data, char _c);
void Splash_KeyPressed(MainData *_data, sfKeyCode _key);
void Splash_KeyReleased(MainData *_data, sfKeyCode _key);
void Splash_MouseButtonScrolled(MainData *_data, float _delta);
void Splash_MouseButtonPressed(MainData *_data, sfMouseButton _button);
void Splash_MouseButtonReleased(MainData *_data, sfMouseButton _button);
void Splash_MouseMoved(MainData *_data);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Splash_HandleEvent(MainData *_data, sfEvent _event)
{
	switch (_event.type)
	{
	case sfEvtTextEntered:
		Splash_TextEntered(_data, (char)_event.text.unicode);
		break;
	case sfEvtKeyPressed:
		Splash_KeyPressed(_data, _event.key.code);
		break;
	case sfEvtKeyReleased:
		Splash_KeyReleased(_data, _event.key.code);
		break;
	case sfEvtMouseWheelScrolled:
		Splash_MouseButtonScrolled(_data, _event.mouseWheelScroll.delta);
		break;
	case sfEvtMouseButtonPressed:
		Splash_MouseButtonPressed(_data, _event.mouseButton.button);
		break;
	case sfEvtMouseButtonReleased:
		Splash_MouseButtonReleased(_data, _event.mouseButton.button);
		break;
	case sfEvtMouseMoved:
		Splash_MouseMoved(_data);
		break;
	default:
		break;
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Splash_TextEntered(MainData *_data, char _c)
{
	char tmp_s[2];
	// Create a shortcut to the state data
	SplashData *state = _data->stateData[SPLASH];

	// Create the string
	tmp_s[0] = _c;
	tmp_s[1] = '\0';

	///printf("Text entered %s\n", tmp_s);
}

void Splash_KeyPressed(MainData *_data, sfKeyCode _key)
{
	// Create a shortcut to the state data
	SplashData *state = _data->stateData[SPLASH];

	state->state = END_SPLASH;
	state->keyIsPressed = TRUE;
	///printf("Key pressed\n");
}

void Splash_KeyReleased(MainData *_data, sfKeyCode _key)
{
	// Create a shortcut to the state data
	SplashData *state = _data->stateData[SPLASH];

	///printf("Key released\n");
}

void Splash_MouseButtonScrolled(MainData *_data, float _delta)
{
	// Create a shortcut to the state data
	SplashData *state = _data->stateData[SPLASH];

	///printf("Mouse button scrolled for %f delta\n", _delta);
}

void Splash_MouseButtonPressed(MainData *_data, sfMouseButton _button)
{
	// Create a shortcut to the state data
	SplashData *state = _data->stateData[SPLASH];
	// Get the mouse position in the screen
	///sfVector2i mousePosition = sfMouse_getPositionRenderWindow(_data->engine.window);
	// Create a shortcut to the key used in keybinding
	///Keybinding *key = _data->keybinding;


}

void Splash_MouseButtonReleased(MainData *_data, sfMouseButton _button)
{
	// Create a shortcut to the state data
	SplashData *state = _data->stateData[SPLASH];
	// Create a shortcut to the key used in keybinding
	///Keybinding *key = _data->keybinding;
}

void Splash_MouseMoved(MainData *_data)
{
	// Create a shortcut to the state data
	SplashData *state = _data->stateData[SPLASH];
	///sfVector2i mousePosition = sfMouse_getPositionRenderWindow(_data->engine.window);

}