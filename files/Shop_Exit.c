#include "Shop.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- MAIN FUNCTION -------------------------------------------------------- //

void Shop_Exit(MainData *_data)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[SHOP];

	for (int i = 0; i < state->players.number; i++)
	{
		if (state->testWeapon[i])
		{
			free(state->testWeapon[i]);
		}
	}

	Laser_DestroyList(state->lasers);

	for (int i = 0; i < WEAPON_OWNER_SIZE; i++)
	{
		List_Destroy(&state->projectiles[i]);
	}
	List_Destroy(&state->explosions);

	// Don't free the whole state, some variables are (or can be) used in game.
	// Complete deletion happens after the state SHOP gave the infos to the state GAME.
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //