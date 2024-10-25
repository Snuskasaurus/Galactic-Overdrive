#include "Overworld.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void DeleteOverworldSprites(OverworldData *_state);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Overworld_Exit(MainData *_data)
{
	OverworldData *state = _data->stateData[OVERWORLD];

	// Delete music
	sfMusic_stop(state->music);
	sfMusic_destroy(state->music);

	DeleteOverworldSprites(state);

	free(state);
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void DeleteOverworldSprites(OverworldData *_state)
{
	sfSprite_destroy(_state->selection.cursor);
	
	for (int i = 0; i < SELECTION_INFO_NB; i++)
	{
		sfSprite_destroy(_state->selection.info[i]);
	}

	sfSprite_destroy(_state->selection.connect);
	sfSprite_destroy(_state->selection.selected);

	for (int i = 0; i < MAX_PLAYER_NUMBER; i++)
	{
		sfSprite_destroy(_state->selection.connected[i]);
	}

	sfSprite_destroy(_state->background);
	
	for (int i = 0; i < NB_SPRITE_SELECT; i++)
	{
		sfSprite_destroy(_state->select[i]);
	}
	
	for (int i = 0; i < MOVE_CONTROLS_NB; i++)
	{
		sfSprite_destroy(_state->moveControls[i]);
	}
	
	for (int i = 0; i < NB_DIF_MODE; i++)
	{
		sfSprite_destroy(_state->difficulty[i]);
	}
}