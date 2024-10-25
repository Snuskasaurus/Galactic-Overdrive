#include "Menu.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void DeleteMenuSprites(MenuData *state);
void DeleteMenuButtons(MenuData *_state);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Menu_Exit(MainData *_data)
{
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];

	DeleteMenuSprites(state);
	DeleteMenuButtons(state);

	// Delete the state from memory
	free(state);
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void DeleteMenuSprites(MenuData *_state)
{
	sfSprite_destroy(_state->background);

	// Achievement tab
	sfSprite_destroy(_state->achievementMenu);
	sfSprite_destroy(_state->achievementMenuBackground);
	sfSprite_destroy(_state->achievementSectionUnlockedBackground);
	sfSprite_destroy(_state->achievementSectionLockedBackground);
	sfSprite_destroy(_state->achievementArrowLeftInput);
	sfSprite_destroy(_state->achievementArrowRightInput);
}

void DeleteMenuButtons(MenuData *_state)
{
	for (int i = 0; i < MENU_SUBSTATES_NUMBER; i++)
	{
		Button_DeleteAll(&_state->buttons[i]);
	}
}