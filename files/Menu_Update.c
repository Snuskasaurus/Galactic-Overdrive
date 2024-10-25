#include "Menu.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void AchievementMenu_Update(MainData *_data);
void ResetCurrentButton(Menu_SubState *_previousSubState, Menu_SubState _currentSubState, int *_currentButton, Buttons buttons[MENU_SUBSTATES_NUMBER]);
void UseJoystickInMenu(MenuData *_state, Engine *_engine);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Menu_Update(MainData *_data, float _dt)
{
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];
	// Get the mouse position in the screen
	sfVector2i mousePosition_i = sfMouse_getPositionRenderWindow(_data->engine.window);
	sfVector2f mousePosition_f = { (float)mousePosition_i.x , (float)mousePosition_i.y };

	Timer_Update(&state->timeSinceInput, _dt);
	UseJoystickInMenu(state, &_data->engine);
	Button_Update(&_data->engine, &state->buttons[state->subState], mousePosition_f);
	AchievementMenu_Update(_data);

	ResetCurrentButton(&state->previousSubState, state->subState, &state->currentButton, state->buttons);
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void ResetCurrentButton(Menu_SubState *_previousSubState, Menu_SubState _currentSubState, int *_currentButton, Buttons buttons[MENU_SUBSTATES_NUMBER])
{
	if ((*_previousSubState) != _currentSubState)
	{
		*_currentButton = 0;

		for (int i = 0; i < MENU_SUBSTATES_NUMBER; i++)
		{
			for (int j = 0; j < buttons[i].number; j++)
			{
				buttons[i].array[j]->currentState = BUTTON_WAITING;
			}
		}
	}

	(*_previousSubState) = _currentSubState;
}

void UseJoystickInMenu(MenuData *_state, Engine *_engine)
{
	if (Timer_Check(&_state->timeSinceInput))
	{
		sfBool hasUpdatedCurrentButton = Menu_ChangeCurrentButton(_engine, _state, -sfJoystick_getAxisPosition(0, sfJoystickY), _state->buttons[_state->subState].number);

		if (hasUpdatedCurrentButton)
		{
			Timer_Reset(&_state->timeSinceInput);
		}
	}
}