#include "Splash.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void DeleteSplashSprites(SplashData *_state);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Splash_Exit(MainData *_data)
{
	// Create a shortcut to the state data
	SplashData *state = _data->stateData[SPLASH];

	DeleteSplashSprites(state);

	// Delete the state from memory
	free(state);
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void DeleteSplashSprites(SplashData *_state)
{
	sfSprite_destroy(_state->logoPixelMatter);
	sfSprite_destroy(_state->logoCreajeux);
	sfSprite_destroy(_state->imgController);
}