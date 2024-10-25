#include "Editor.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- MAIN FUNCTION -------------------------------------------------------- //

void Editor_Exit(MainData *_data)
{
	// Create a shortcut to the state data
	EditorData *state = _data->stateData[EDITOR];

	// Free TerrainRepetition array
	free(state->tabTerrainRepetition);
	state->tabTerrainRepetition = NULL;

	// Free paralaxes
	List_Destroy(&state->listParallax);

	// Free terrains
	if (state->level.map.nbTerrains > 0)
	{
		free(state->level.map.terrains);
		state->level.map.terrains = NULL;
	}

	// Delete the state from memory
	free(state);
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //